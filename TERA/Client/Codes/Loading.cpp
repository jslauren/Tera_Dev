#include "stdafx.h"
#include "..\Headers\Loading.h"
#include "Component_Manager.h"
#include "Management.h"
#include "Object_Manager.h"
#include "Player.h"

_USING(Client)

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
{
	Safe_AddRef(m_pComponent_Manager);
}

HRESULT CLoading::Ready_Loading(SCENEID eSceneID)
{
//	m_eSceneID = (SCENEID)CManagement::GetInstance()->Get_PreScene();
	m_eSceneID = eSceneID;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 1, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return NOERROR;
}

_uint CLoading::Thread_Main(void * pArg)
{
	CLoading*	pLoading = (CLoading*)pArg;

	EnterCriticalSection(&pLoading->m_CS);

	HRESULT		hr;

	switch (pLoading->Get_SceneID())
	{
	case SCENE_LOGO:
	case SCENE_STAGE:
		hr = pLoading->Ready_Stage_Component();
		break;
	case SCENE_DRAGON:
		hr = pLoading->Ready_Dragon_Component();
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
	
	// 씬 전환 시 컴포넌트 프로토타입에서 추가 형태가 SCENE_STATIC인 애들은 제거하기 위해,
	// 아래와 같은 구문을 적용 함.
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player", 0));

	//if (pPlayer != nullptr)
	//	m_bIsAlreadyLoaded = pPlayer->Get_PreventPrototypeInfo();
	m_bIsAlreadyLoaded = CManagement::GetInstance()->Get_PreventPrototypeLoadInfo();

	if (m_bIsAlreadyLoaded == false)
		m_iTotalRsrcNum = 86;
	else
		m_iTotalRsrcNum = 6;

	m_iCurrentRsrcNum = 0;
	m_iComplete = 0;

	// [Shader]
	// For.Component_Shader_Sky 
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Sky", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Shader_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Shader_Mesh
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Navigation]
	// For.Component_Navigation_Stage
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Navigation_Stage", CNavigation::Create(m_pGraphic_Device, L"../../Data/Town.NaviDat"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Buffer & Texture]
	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (m_bIsAlreadyLoaded == true)
		return NOERROR;

	// For.Com_Buffer
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Point_Board
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Point_Board", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Hp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Hp", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Mp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Mp", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Mp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Test", CBuffer_RcCol::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Point_Board
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Point_Board", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/PlayerPoint/PlayerPoint.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Hp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Hp", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/PlayerPoint/HP.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Mp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Mp", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/PlayerPoint/MP.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();
	
	// For.Component_Buffer_UI_SkillBoard
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_SkillBoard", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_SkillFilter
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Point_Boss_Board
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Point_Boss_Board", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Boss_Hp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Boss_Hp", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_SkillBoard
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_SkillBoard", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/SkillBoard/SkillBoard.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_SkillFilter
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_SkillFilter", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/SkillBoard/SkillFilter.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Point_Boss_Board
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Point_Boss_Board", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/BossPoint/Boss_HP_Bar.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Boss_Hp
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Boss_Hp", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/BossPoint/Boss_Hp.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Dialog
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Dialog", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Dialog
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Dialog", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Quest/Dialog.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();
	
	// For.Component_Buffer_UI_Inventory
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Inventory", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();
	
	// For.Component_Texture_UI_Inventory
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Inventory", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Inventory/Inventory.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Buffer_UI_Inventory
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Mouse", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_UI_Inventory
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Mouse", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Mouse/Mouse.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	//// For.Component_Texture_Terrain
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Common_BG/Texture2D/CM_Floor_08_Diff.tga"))))
	//	return E_FAIL;

	// [Mesh_Dynamic]
	// For.Component_Mesh_Player
	// [메인 통 본]
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Bone", CMesh_Dynamic_Bone::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Bone/", L"Bone_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [추가 될 Parts 들]
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Head00", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Head/00/", L"Head00_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Head01", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Head/01/", L"Head01_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Head02", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Head/02/", L"Head02_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Head03", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Head/03/", L"Head03_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();


	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Body15", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/15/", L"Body15_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Body16", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/16/", L"Body16_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Body17", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/17/", L"Body17_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Body18", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/18/", L"Body18_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_BodyH20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/H20/", L"BodyH20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_BodyL20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/L20/", L"BodyL20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_BodyR20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/R20/", L"BodyR20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_BodyR21", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/R21/", L"BodyR21_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_BodyR29", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Body/R29/", L"BodyR29_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();


	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Hand15", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/15/", L"Hand15_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Hand16", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/16/", L"Hand16_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Hand17", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/17/", L"Hand17_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Hand18", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/18/", L"Hand18_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_HandH20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/H20/", L"HandH20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_HandL20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/L20/", L"HandL20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_HandR20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/R20/", L"HandR20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_HandR21", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/R21/", L"HandR21_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_HandR29", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Hand/R29/", L"HandR29_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();


	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Leg15", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/15/", L"Leg15_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Leg16", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/16/", L"Leg16_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Leg17", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/17/", L"Leg17_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Leg18", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/18/", L"Leg18_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_LegH20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/H20/", L"LegH20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_LegL20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/L20/", L"LegL20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_LegR20", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/R20/", L"LegR20_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_LegR21", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/R21/", L"LegR21_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_LegR29", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Leg/R29/", L"LegR29_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();


	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Tail00", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Tail/00/", L"Tail00_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Tail01", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Tail/01/", L"Tail01_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Tail02", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Tail/02/", L"Tail02_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Player_Tail03", CMesh_Dynamic_Parts::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Player/Parts/Tail/03/", L"Tail03_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [NPC Mesh]
	// For.Component_Mesh_QuestNPC
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_QuestNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/QuestNPC/", L"QuestNPC_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_GroceryNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/StoreNPC/", L"StoreNPC_Grocery_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_WeaponNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/StoreNPC/", L"StoreNPC_Weapon_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_ContentsNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/ContentsNPC/", L"ContentsNPC_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_GuardNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/GuardNPC/", L"GuardNPC_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_CartNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Cart/", L"Cart_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_BoyNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Boy/", L"Boy_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_GirlNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Girl/", L"Girl_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_CatNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Cat/", L"Cat_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_DogNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Dog/", L"Dog_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_LionNPC", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Lion/", L"Lion_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_PoporiNPC_A", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Popori/", L"PoporiA_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_PoporiNPC_B", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Popori/", L"PoporiB_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_PoporiNPC_C", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/NPC/Popori/", L"PoporiC_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Q_Mark]
	// For.Component_Mesh_QMark_Start
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_QMark_Start", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/QMark/", L"Qmark_Start_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Mesh_QMark_Ongoing
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_QMark_Ongoing", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/QMark/", L"Qmark_Ongoing_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Mesh_QMark_Reward
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_QMark_Reward", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/QMark/", L"Qmark_Reward_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Mesh_Static]
	// For.Component_Mesh_Weapon
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Mesh_Weapon", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/Weapon/1/", L"Popori_Weapon_B.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Collider]
	// For.Component_Collider_AABB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_AABB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Collider_OBB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_OBB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Collider_Sphere
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Collider_Sphere", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Frustum]
	// For.Component_Frustum
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Frustum", CFrustum::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	return NOERROR;
}

HRESULT CLoading::Ready_Dragon_Component()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_iTotalRsrcNum = 18;
	m_iCurrentRsrcNum = 0;
	m_iComplete = 0;

	// [Shader]
	// For.Component_Shader_Sky 
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Shader_Sky", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Shader_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Shader_Mesh
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Shader_Effect
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Shader_Effect", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Effect.fx"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Buffer & Texture]
	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_HitEffect
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_HitEffect", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Effect/Hit/Hit%d.png", 16))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Texture_SwordScarEffect
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_SwordScarEffect", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Effect/SwordScar/SwordScar.png"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Damage_One", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Damage_Ten", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Damage_Hndr", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Damage_Thsn", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;
	CalculatedCompleteNumber();

	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_UI_Damage", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/DamageFont/DamageFont_%d.png", 10))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Mesh]
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Mesh_Arkus", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Monster/Arkus/", L"Arkus.X"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Collider]
	// For.Component_Collider_AABB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Collider_AABB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_AABB))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Collider_OBB
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Collider_OBB", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_OBB))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// For.Component_Collider_Sphere
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Collider_Sphere", CCollider::Create(m_pGraphic_Device, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	CalculatedCompleteNumber();

	// [Navigation]
	// For.Component_Navigation_Stage
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_DRAGON, L"Component_Navigation_Dragon", CNavigation::Create(m_pGraphic_Device, L"../../Data/Arena.NaviDat"))))
		return E_FAIL;
	CalculatedCompleteNumber();

	return NOERROR;
}

void CLoading::CalculatedCompleteNumber()
{
	m_iCurrentRsrcNum++;
	m_iComplete = (_uint)(100 * ((_float)m_iCurrentRsrcNum / m_iTotalRsrcNum));
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
