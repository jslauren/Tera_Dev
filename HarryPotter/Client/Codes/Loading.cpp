#include "stdafx.h"
#include "..\Headers\Loading.h"
#include "Component_Manager.h"

_USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
{
	Safe_AddRef(m_pComponent_Manager);
}

HRESULT CLoading::Ready_Loading(SCENEID eSceneID)
{
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return NOERROR;
}

_uint CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	EnterCriticalSection(&pLoading->m_CS);

	HRESULT			hr;

	switch (pLoading->Get_SceneID())
	{
	case SCENE_STAGE:
		hr = pLoading->Ready_Stage_Component();
		break;
	case SCENE_BOSS:
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&pLoading->m_CS);

	return _uint(0);
}

HRESULT CLoading::Ready_Stage_Component()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	
	// Total = 13개.

	m_iComplete = 0;

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

	m_iComplete = 21;

	// [Buffer & Texture]
	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;

	m_iComplete = 35;

	// [Mesh_Dynamic]
	// For.Component_Mesh_Player
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Hermione/", L"Hermione.x"))))
		return E_FAIL;

	// 여기에 Component_Mesh_Monster 이 값이 몬스터 클래스의 Add_Component함수에 두 번째 인자로 들어가면서,
	// 몬스터 클래스만 있기때문에 실바만 나온다. 추후 각 오브젝트들이 추가될 때마다 클래스를 생성해 주어야 한다.

	// For.Component_Mesh_Monster
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Monster", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/sylva/", L"sylva.X"))))
		return E_FAIL;

	m_iComplete = 49;

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

	m_iComplete = 70;

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

	m_iComplete = 100;

	return NOERROR;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, SCENEID eSceneID)
{
	CLoading* pInstance = new CLoading(pGraphic_Device);

	if (FAILED(pInstance->Ready_Loading(eSceneID)))
	{
		_MSGBOX("CLoading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pComponent_Manager);
}
