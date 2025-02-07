#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Camera_Dynamic.h"
#include "Camera_Static.h"
#include "Light_Manager.h"
#include "Scene_Loading.h"
#include "TerrainObject.h"
#include "Scene_Dragon.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "Input_Device.h"
#include "Management.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Player.h"
#include "Weapon.h"
#include "Arkus.h"

#include "UI_PlayerPoint.h"
#include "UI_MouseCursor.h"
#include "UI_SkillBoard.h"
#include "UI_Inventory.h"
#include "UI_Loading.h"
#include "UI_Dialog.h"

#include "ContentsNPC.h"
#include "PoporiKidA.h"
#include "PoporiKidB.h"
#include "PoporiKidC.h"
#include "GroceryNPC.h"
#include "WeaponNPC.h"
#include "GuardNPC.h"
#include "QuestNPC.h"
#include "CartNPC.h"
#include "GirlNPC.h"
#include "LionNPC.h"
#include "BoyNPC.h"
#include "CatNPC.h"
#include "DogNPC.h"
#include "QMark.h"


#define	NEAR			0.2f
#define FAR				1000.f
#define ASPECT			_float(g_iWinCX) / g_iWinCY
#define FOV(X)			D3DXToRadian(X)
#define	AXIS_Y			_vec3(0.f, 1.f, 0.f)

_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
	ZeroMemory(m_szPlayerPos, sizeof(_tchar) * 128);
}

HRESULT CScene_Stage::Ready_Scene()
{
	CSoundManager::GetInstance()->Stop_AllSound();
	CSoundManager::GetInstance()->Play_BGM("Accarume_Village.ogg");

	m_bIsAlreadyLoaded = CManagement::GetInstance()->Get_PreventPrototypeLoadInfo();

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

	// For.Layer_Camera
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	// For.Layer_Player
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	// For.Layer_UI
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	// For.Layer_NPC
	if (FAILED(Ready_Layer_NPC(L"Layer_NPC")))
		return E_FAIL;

	// For.Layer_Weapon
	if (FAILED(Ready_Layer_Weapon(L"Layer_Weapon")))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	// For.Initialize_Setting
	if (FAILED(Init_Setting()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Init_Setting()
{
	// 여기서 현재 씬을 정해준다.
	CManagement::GetInstance()->Set_CurrentScene(SCENE_STAGE);
	CManagement::GetInstance()->Set_FirstLoadingInfo(false);

	m_pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));
	Safe_AddRef(m_pPlayer);

	m_pPlayer->Get_Transform()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(12.f, 0.f, 233.f));

	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Stage::LateUpdate_Scene(const _float & fTimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	pManagement->AddRef();

	_bool bSceneChangeStagetoDragon = dynamic_cast<CCartNPC*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_NPC", 4))->Get_SceneChangeAvailableInfo();

	if ((CInput_Device::GetInstance()->GetDIKeyState(DIK_F11) & 0x80) ||
		bSceneChangeStagetoDragon == true)
	{
		bSceneChangeStagetoDragon = false;

		HRESULT		hr;
		if (S_OK == (hr = (pManagement->SetUp_CurrentScene(CScene_Loading::Create(m_pGraphic_Device, SCENE_DRAGON), SCENE_LOADING))))
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

HRESULT CScene_Stage::Render_Scene()
{
	_vec3 vPlayerPos = *m_pPlayer->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	
//	wsprintf(m_szPlayerPos, L"%d, %d, %d", (_int)vPlayerPos.x, (_int)vPlayerPos.y, (_int)vPlayerPos.z);

	POINT ptMousePos;
	GetCursorPos(&ptMousePos);		
	ScreenToClient(g_hWnd, &ptMousePos);
	wsprintf(m_szPlayerPos, L"%d, %d", (_int)ptMousePos.x, (_int)ptMousePos.y);
	CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.5) - 50.f, g_iWinCY - 30.f, 0.f), m_szPlayerPos);

	return CScene::Render_Scene();
}

HRESULT CScene_Stage::Ready_TerrainLoad()
{
	const wstring& wstrPath = L"../../Data/Town.Map";

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
		if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(m_pGraphic_Device, pTerrainData.iNumVtxX, pTerrainData.iNumVtxZ, pTerrainData.fInterval, pTerrainData.fDetail))))
			return E_FAIL;

		// For.Component_Texture_Terrain
		if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, imgPathTemp))))
			return E_FAIL;

	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_MeshLoad()
{
	const wstring& wstrPath = L"../../Data/Town.MeshDat";

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
			if (nullptr == m_pComponent_Manager->Find_Component_Prototype(SCENE_STAGE, tObjMeshData.strComProtoTag.c_str()))
			{
				if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, tObjMeshData.strComProtoTag.c_str(), CMesh_Static::Create(m_pGraphic_Device, tObjMeshData.strFullPath.c_str(), strXfileName.c_str()))))
					return E_FAIL;
			}

			// 오브젝트 정보 추가.
			if (nullptr == CObject_Manager::GetInstance()->Find_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str()))
			{
				if (FAILED(Add_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), CTerrainObject::Create(m_pGraphic_Device))))
					return E_FAIL;
			}

			if (FAILED(Add_Object(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), SCENE_STAGE, L"Layer_TerrainData", &tObjMeshData)))
				return E_FAIL;
		}
		else
		{
			// 컴포넌트 정보 추가.
			if (nullptr == m_pComponent_Manager->Find_Component_Prototype(SCENE_STAGE, tObjMeshData.strComProtoTag.c_str()))
			{
				if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, tObjMeshData.strComProtoTag.c_str(), CMesh_Dynamic::Create(m_pGraphic_Device, tObjMeshData.strFullPath.c_str(), strXfileName.c_str()))))
					return E_FAIL;
			}

			// 오브젝트 정보 추가.
			if (nullptr == CObject_Manager::GetInstance()->Find_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str()))
			{
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
				if (lstrcmp(tObjMeshData.strObjProtoTag.c_str(), L"몬스터 X파일 이름") == 0)
				{
					//if (FAILED(Add_Object_Prototype(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), /*해당 몬스터 클래스명*/::Create(m_pGraphic_Device))))
					//	return E_FAIL;
				}
			}
			
			if (FAILED(Add_Object(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), SCENE_STAGE, L"Layer_Monster", &tObjMeshData)))
				return E_FAIL;
		}
	}

	CloseHandle(hFile);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_LightInfo()
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
	LightInfo.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;

	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Position = _vec3(480.f, 15.f, 321.f);
	LightInfo.Range = 60.0f;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.1f, 0.1f, 1.f);
	LightInfo.Specular = LightInfo.Diffuse;

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;

	Safe_Release(pLight_Manager);

	////((L.Specular * M.Specular) * 스펙큘러의 세기) + (L.Diffuse * M.Diffuse) * 명암 + (L.Ambient * M.Ambient)
	//m_pGraphic_Device->SetLight(0, &LightInfo);
	//m_pGraphic_Device->LightEnable(0, TRUE);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Component_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Stage::Ready_GameObject_Prototype()
{
	// For.GameObject_SkyBox
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Camera_Dynamic
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Camera_Dynamic", CCamera_Dynamic::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Camera_Static
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Camera_Static", CCamera_Static::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (m_bIsAlreadyLoaded == true)
		return NOERROR;

	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Weapon
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Weapon", CWeapon::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Quest
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Quest", CQuestNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Grocery
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Grocery", CGroceryNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Weapon
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Weapon", CWeaponNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Contents
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Contents", CContentsNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Guard
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Guard", CGuardNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Cart
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Cart", CCartNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Boy
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Boy", CBoyNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Girl
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Girl", CGirlNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Cat
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Cat", CCatNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Dog
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Dog", CDogNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Lion
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Lion", CLionNPC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Popori_A
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Popori_A", CPoporiKidA::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Popori_B
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Popori_B", CPoporiKidB::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_NPC_Popori_C
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_NPC_Popori_C", CPoporiKidC::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_PlayerPoint
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_PlayerPoint", CUI_PlayerPoint::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_SkillBoard
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_SkillBoard", CUI_SkillBoard::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_QMark
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_QMark", CQMark::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_Dialog
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_Dialog", CUI_Dialog::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_Inventory
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_Inventory", CUI_Inventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI_Mouse
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_UI_Mouse", CUI_MouseCursor::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	// For.Camera_Dynamic
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Camera_Dynamic", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 5, -5), _vec3(0, 0, 0), AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
		return E_FAIL;

	// For.Camera_Static
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Camera_Static", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 5, -5), _vec3(0, 0, 0), AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	if (m_bIsAlreadyLoaded == true)
		return NOERROR;

	// For.Player
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Player", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_PlayerPoint", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_SkillBoard", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_Inventory", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_Mouse", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_QMark", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_UI_Dialog", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_NPC(const _tchar * pLayerTag)
{
	if (m_bIsAlreadyLoaded != true)
	{
		// For.GameObject_NPC_Quest
		if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Quest", SCENE_STATIC, pLayerTag)))
			return E_FAIL;
	}

	// For.GameObject_NPC_Grocery
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Grocery", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Weapon
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Weapon", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Contents
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Contents", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Guard
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Guard", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Cart
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Cart", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Boy
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Boy", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Girl
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Girl", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Cat
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Cat", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Dog
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Dog", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Lion
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Lion", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Popori_A
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Popori_A", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Popori_B
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Popori_B", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.GameObject_NPC_Popori_C
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_NPC_Popori_C", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Weapon(const _tchar * pLayerTag)
{
	if (m_bIsAlreadyLoaded == true)
		return NOERROR;

	// For.Weapon
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Weapon", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.SkyBox
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_SkyBox", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Terrain", SCENE_STAGE, pLayerTag, (void*)&m_fDetail)))
		return E_FAIL;

	return NOERROR;
}

CScene_Stage * CScene_Stage::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Stage* pInstance = new CScene_Stage(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Stage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScene_Stage::Free()
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	Safe_Release(m_pPlayer);

	if (FAILED(m_pObject_Manager->Clear_Object(SCENE_STAGE)))
		return;

	if (FAILED(m_pObject_Manager->Clear_Prototype(SCENE_STAGE)))
		return;

	if (FAILED(m_pComponent_Manager->Clear_Component_Prototype(SCENE_STAGE)))
		return;

	if (FAILED(CLight_Manager::GetInstance()->DeleteLightAll()))
		return;

	CScene::Free();
}


 