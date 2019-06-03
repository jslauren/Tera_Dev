#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Camera_Dynamic.h"
#include "Camera_Static.h"
#include "Light_Manager.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Player.h"
#include "Weapon.h"
#include "Monster.h"

#define	NEAR			0.2f
#define FAR				500.f
#define ASPECT			_float(g_iWinCX) / g_iWinCY
#define FOV(X)			D3DXToRadian(X)
#define	AXIS_Y			_vec3(0.f, 1.f, 0.f)

_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage::Ready_Scene()
{
	// For.Terrain Data Load
	if (FAILED(Ready_TerrainLoad()))
		return E_FAIL;

	// For.Light Setting
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	// �ΰ������ ����� ����������Ʈ���� ����.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// �ΰ������ ����� ��ü���� ������ �غ��س��´�.
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

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	// For.Layer_Monster
	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;

	// For.Layer_UI
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	return NOERROR;
}

_int CScene_Stage::Update_Scene(const _float & fTimeDelta)
{
	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Stage::LateUpdate_Scene(const _float & fTimeDelta)
{
	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Stage::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CScene_Stage::Ready_TerrainLoad()
{
	const wstring& wstrPath = L"../../Data/3.Map";

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
	const wstring& wstrPath = L"../../Data/9.MeshDat";

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

		//ObjectName.TrimLeft(L"Layer_");

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strComProtoTag = szTextTemp;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strFullPath = szTextTemp;

		if (0 == dwByte)
			break;

		// For.Monster
		if (FAILED(Add_Object(SCENE_STAGE, tObjMeshData.strObjProtoTag.c_str(), SCENE_STAGE, L"Layer_Monster", (void*)tObjMeshData.matWorld)))
			return E_FAIL;

		//CLayer* pStaticObjLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);
		//dynamic_cast<CStaticObject*>(pStaticObjLayer->Get_ObjectList().back())->SetState(*(_vec3*)&tObjMeshData.matWorld.m[3], _vec3(1.f, 1.f, 1.f));

		//vector<OBJECTMESHDATA> vObjMeshData;

		//auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strObjectName));

		//if (iter == CDataManager::GetInstance()->m_MapMeshData.end())
		//{
		//	vObjMeshData.push_back(tObjMeshData);
		//	CDataManager::GetInstance()->m_MapMeshData.emplace(ObjectName, vObjMeshData);
		//}
		//else
		//	(*iter).second.push_back(tObjMeshData);
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
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;

	Safe_Release(pLight_Manager);

	////((L.Specular * M.Specular) * ����ŧ���� ����) + (L.Diffuse * M.Diffuse) * ��� + (L.Ambient * M.Ambient)
	//m_pGraphic_Device->SetLight(0, &LightInfo);
	//m_pGraphic_Device->LightEnable(0, TRUE);

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	// [Shader]
	// For.Component_Shader_Sky 
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Sky", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;

	// For.Component_Shader_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	
	// For.Component_Shader_Mesh
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;

	// [Buffer & Texture]
	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;

	// [Mesh_Dynamic]
	// For.Component_Mesh_Player
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Hermione/", L"Hermione.x"))))
		return E_FAIL;

	// ���⿡ Component_Mesh_Monster �� ���� ���� Ŭ������ Add_Component�Լ��� �� ��° ���ڷ� ���鼭,
	// ���� Ŭ������ �ֱ⶧���� �ǹٸ� ���´�. ���� �� ������Ʈ���� �߰��� ������ Ŭ������ ������ �־�� �Ѵ�.

	// For.Component_Mesh_Monster
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Monster", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/sylva/", L"sylva.X"))))
		return E_FAIL;

	// [Mesh_Static]
	// For.Component_Mesh_Weapon
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Weapon", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/Wand/", L"Wand.x"))))
		return E_FAIL;

	// For.Component_Mesh_TombStone
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_TombStone", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/TombStone/", L"TombStone.x"))))
		return E_FAIL;

	// For.Component_Mesh_Tiger
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Tiger", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/Tiger/", L"Tiger.x"))))
		return E_FAIL;	

	// [Collider]
	// For.Component_Collider_AABB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Collider_AABB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB))))
		return E_FAIL;

	// For.Component_Collider_OBB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Collider_OBB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))
		return E_FAIL;

	// For.Component_Collider_Sphere
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Collider_Sphere", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	
	return NOERROR;
}

HRESULT CScene_Stage::Ready_GameObject_Prototype()
{
	// For.GameObject_SkyBox
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Camera_Static
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Camera_Static", CCamera_Static::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	//// For.GameObject_UI
	//if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_UI", CUI::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Weapon
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Weapon", CWeapon::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Monster
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Monster", CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_tiger
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_tiger", CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_TombStone
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_TombStone", CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_sylva
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_sylva", CMonster::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Player(const _tchar * pLayerTag)
{
	// For.Player
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Player", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	// For.Weapon
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Weapon", SCENE_STATIC, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	// For.Camera_Dynamic
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Camera_Dynamic", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 5, -5), _vec3(0, 0, 0), AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
		return E_FAIL;

	// For.Camera_Static
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Camera_Static", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0, 5, -5), _vec3(0, 0, 0), AXIS_Y, FOV(60.0f), ASPECT, NEAR, FAR))))
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

HRESULT CScene_Stage::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	// For.UI
	//if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_UI", SCENE_STAGE, pLayerTag)))
	//	return E_FAIL;

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

	if (FAILED(m_pObject_Manager->Clear_Object(SCENE_STAGE)))
		return;

	if (FAILED(m_pObject_Manager->Clear_Prototype(SCENE_STAGE)))
		return;

	if (FAILED(m_pComponent_Manager->Clear_Component_Prototype(SCENE_STAGE)))
		return;

	CScene::Free();
}


