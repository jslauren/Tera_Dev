#include "stdafx.h"
#include "..\Headers\Scene_Dragon.h"
#include "Camera_Dynamic.h"
#include "Camera_Static.h"
#include "Light_Manager.h"
#include "SkyBox_Dragon.h"
#include "Terrain_Dragon.h"
#include "Management.h"
#include "Scene_Loading.h"
#include "Player.h"
#include "Weapon.h"
#include "Monster.h"
#include "Arkus.h"
#include "TerrainObject.h"
#include "FontManager.h"

#include "UI_PlayerPoint.h"
#include "UI_BossPoint.h"
#include "UI_SkillBoard.h"
#include "UI_Loading.h"
#include "UI_DamageTexture.h"
#include "UI_DamageFont_Manager.h"

#define	NEAR			0.2f
#define FAR				1000.f
#define ASPECT			_float(g_iWinCX) / g_iWinCY
#define FOV(X)			D3DXToRadian(X)
#define	AXIS_Y			_vec3(0.f, 1.f, 0.f)

_USING(Client)

CScene_Dragon::CScene_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Dragon::Ready_Scene()
{
	// For.Terrain Data Load
	if (FAILED(Ready_TerrainLoad()))
		return E_FAIL;

	// For.Light Setting
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	// 로고씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// For.Mesh Data Load
	if (FAILED(Ready_MeshLoad()))
		return E_FAIL;

	// For.Layer_UI
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	// For.Layer_Player
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	// For.Layer_Weapon
	if (FAILED(Ready_Layer_Weapon(L"Layer_Weapon")))
		return E_FAIL;
	
	// For.Layer_Monster
	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	// For.Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;
	
	SetCutSceneEvent();

	// 가즈아 릴리즈 모드!!!
	int i = 0;

	return NOERROR;
}

_int CScene_Dragon::Update_Scene(const _float & fTimeDelta)
{
	if (m_bIsSceneFirstPlay == true)
	{
		dynamic_cast<CCamera_Dynamic*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 0))->Set_CurrentScene(SCENE_DRAGON);
		m_bIsSceneFirstPlay = false;
	}

	CUI_DamageFont_Manager::GetInstance()->Update_DamageFont(fTimeDelta);

	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Dragon::LateUpdate_Scene(const _float & fTimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	pManagement->AddRef();

	_bool bSceneChangeDragonToStage = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster", 0))->Get_SceneChangeAvailableInfo();

	if (GetKeyState('L') & 0x8000 ||
		bSceneChangeDragonToStage == true)
	{
		// 씬 전환 시 로딩 쓰레드를 쓰는데,
		// 이 때 기존 전환하면서 추가했던 Static 타입의 컴포넌트 프로토타입을
		// 다시 로딩하지 않기 위해 처리하는 구문.
		CManagement::GetInstance()->Set_PreventPrototypeLoadInfo(true);
		dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_CutSceneInfo(false);

	//	dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player", 0))->Set_PreventPrototypeInfo(true);

		HRESULT		hr;
		if (S_OK == (hr = (pManagement->SetUp_CurrentScene(CScene_Loading::Create(m_pGraphic_Device, SCENE_STAGE), SCENE_LOADING))))
		{
			Safe_Release(pManagement);
			return 0;
		}
		if (FAILED(hr))
		{
			Safe_Release(pManagement);
			return -1;
		}

		Safe_Release(pManagement);
		return 0;
	}

	Safe_Release(pManagement);

	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Dragon::Render_Scene()
{
	CUI_DamageFont_Manager::GetInstance()->Render_DamageFont();

	return CScene::Render_Scene();
}

HRESULT CScene_Dragon::Ready_TerrainLoad()
{
	const wstring& wstrPath = L"../../Data/Arena.Map";

	HANDLE		hFile = CreateFile(wstrPath.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD	dwByte = 0;

	while (true)
	{
		TERRAINDATA	pTerrainData;// = new TERRAINDATA;
		_int	iImgPathLength = 0;
		_tchar	imgPathTemp[MAX_PATH] = L"";

		ReadFile(hFile, &pTerrainData.iNumVtxX, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &pTerrainData.iNumVtxZ, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &pTerrainData.fInterval, sizeof(_float), &dwByte, NULL);
		ReadFile(hFile, &pTerrainData.fDetail, sizeof(_float), &dwByte, NULL);
		ReadFile(hFile, &iImgPathLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, imgPathTemp, sizeof(_tchar) * iImgPathLength, &dwByte, NULL);

		if (dwByte == 0)
			break;

		m_fDetail = pTerrainData.fDetail;

		// For.Component_Buffer_Terrain
		if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(m_pGraphic_Device, pTerrainData.iNumVtxX, pTerrainData.iNumVtxZ, pTerrainData.fInterval, pTerrainData.fDetail))))
			return E_FAIL;

		// For.Component_Texture_Terrain
		if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, imgPathTemp))))
			return E_FAIL;

	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_MeshLoad()
{
	const wstring& wstrPath = L"../../Data/Arena.MeshDat";

	HANDLE		hFile = CreateFile(wstrPath.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD	dwByte = 0;

	while (true)
	{
		wstring		ObjectName;
		wstring		strXfileName;

		_int	iTextLength = 0;
		_tchar	szTextTemp[MAX_PATH] = L"";

		OBJECTMESHDATA tObjMeshData;

		ReadFile(hFile, &tObjMeshData.bIsStaticMesh, sizeof(_bool), &dwByte, NULL);
		ReadFile(hFile, &tObjMeshData.matWorld, sizeof(_matrix), &dwByte, NULL);

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strObjProtoTag = szTextTemp;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strLayerTag = szTextTemp;
		ObjectName = tObjMeshData.strLayerTag.c_str();

		_int iStrLength = ObjectName.length();

		if (ObjectName.length() != 0)
			ObjectName = ObjectName.substr(6, iStrLength);

		strXfileName = ObjectName;
		strXfileName += L".X";

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strComProtoTag = szTextTemp;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strFullPath = szTextTemp;

		//		m_MapMeshData.emplace(ObjectName, tObjMeshData);

		if (0 == dwByte)
			break;

		if (tObjMeshData.bIsStaticMesh == true)
		{
			// 컴포넌트 정보 추가.
			if (nullptr == m_pComponent_Manager->Find_Component_Prototype(SCENE_DRAGON, tObjMeshData.strComProtoTag.c_str()))
			{
				if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, tObjMeshData.strComProtoTag.c_str(), CMesh_Static::Create(m_pGraphic_Device, tObjMeshData.strFullPath.c_str(), strXfileName.c_str()))))
					return E_FAIL;
			}

			// 오브젝트 정보 추가.
			if (nullptr == CObject_Manager::GetInstance()->Find_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str()))
			{
				if (FAILED(Add_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), CTerrainObject::Create(m_pGraphic_Device))))
					return E_FAIL;
			}

			if (FAILED(Add_Object(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), SCENE_DRAGON, L"Layer_TerrainData", &tObjMeshData)))
				return E_FAIL;
		}
		else
		{
			// 컴포넌트 정보 추가.
			if (nullptr == m_pComponent_Manager->Find_Component_Prototype(SCENE_DRAGON, tObjMeshData.strComProtoTag.c_str()))
			{
				if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, tObjMeshData.strComProtoTag.c_str(), CMesh_Dynamic::Create(m_pGraphic_Device, tObjMeshData.strFullPath.c_str(), strXfileName.c_str()))))
					return E_FAIL;
			}

			// 오브젝트 정보 추가.
			if (nullptr == CObject_Manager::GetInstance()->Find_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str()))
			{
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
			}

			if (FAILED(Add_Object(SCENE_DRAGON, tObjMeshData.strObjProtoTag.c_str(), SCENE_DRAGON, L"Layer_Monster", &tObjMeshData)))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_LightInfo()
{
	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 0.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;

	Safe_Release(pLight_Manager);

	////((L.Specular * M.Specular) * 스펙큘러의 세기) + (L.Diffuse * M.Diffuse) * 명암 + (L.Ambient * M.Ambient)
	//m_pGraphic_Device->SetLight(0, &LightInfo);
	//m_pGraphic_Device->LightEnable(0, TRUE);

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Component_Prototype()
{
	// 로딩 쓰레드로 가버림 헿

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_GameObject_Prototype()
{
	// For.GameObject_SkyBox
	if (FAILED(Add_Object_Prototype(SCENE_DRAGON, L"GameObject_SkyBox", CSkyBox_Dragon::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Camera_Dynamic
	if (FAILED(Add_Object_Prototype(SCENE_DRAGON, L"GameObject_Camera_Dynamic", CCamera_Dynamic::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Camera_Static
	if (FAILED(Add_Object_Prototype(SCENE_DRAGON, L"GameObject_Camera_Static", CCamera_Static::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_DRAGON, L"GameObject_Terrain", CTerrain_Dragon::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_BossPoint
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_BossPoint", CUI_BossPoint::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Arkus
	if (FAILED(Add_Object_Prototype(SCENE_DRAGON, L"GameObject_Arkus", CArkus::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_UI(const _tchar * pLayerTag)
{
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_BossPoint", SCENE_DRAGON, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_Player(const _tchar * pLayerTag)
{
	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_Weapon(const _tchar * pLayerTag)
{
	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	if (FAILED(Add_Object(SCENE_DRAGON, L"GameObject_Arkus", SCENE_DRAGON, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	// For.Camera_Dynamic
	if (FAILED(Add_Object(SCENE_DRAGON, L"GameObject_Camera_Dynamic", SCENE_DRAGON, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 5, -5), _vec3(0, 0, 0), AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
		return E_FAIL;

	_vec3 vMonsterPos = *dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	// For.Camera_Static
	if (FAILED(Add_Object(SCENE_DRAGON, L"GameObject_Camera_Static", SCENE_DRAGON, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 0, -5), vMonsterPos, AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Dragon::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.SkyBox
	if (FAILED(Add_Object(SCENE_DRAGON, L"GameObject_SkyBox", SCENE_DRAGON, pLayerTag)))
		return E_FAIL;

	// For.Terrain
	if (FAILED(Add_Object(SCENE_DRAGON, L"GameObject_Terrain", SCENE_DRAGON, pLayerTag, (void*)&m_fDetail)))
		return E_FAIL;

	return NOERROR;
}

void CScene_Dragon::SetCutSceneEvent()
{
	dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 1))->Set_TurnOnStaticCam(false);
	dynamic_cast<CCamera_Dynamic*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 0))->Set_TurnOnDynamicCam(true);
	dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_CutSceneInfo(true);
}

CScene_Dragon * CScene_Dragon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Dragon* pInstance = new CScene_Dragon(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Dragon Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Dragon::Free()
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	if (FAILED(m_pObject_Manager->Clear_Object(SCENE_DRAGON)))
		return;

	if (FAILED(m_pObject_Manager->Clear_Prototype(SCENE_DRAGON)))
		return;

	if (FAILED(m_pComponent_Manager->Clear_Component_Prototype(SCENE_DRAGON)))
		return;

	if (FAILED(CLight_Manager::GetInstance()->DeleteLightAll()))
		return;

	CScene::Free();
}
