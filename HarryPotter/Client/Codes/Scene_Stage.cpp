#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"
#include "Terrain.h"
#include "Camera_Dynamic.h"
#include "UI.h"
#include "Player.h"
#include "Monster.h"
#include "SkyBox.h"
#include "Weapon.h"
#include "Light_Manager.h"

_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage::Ready_Scene()
{
	// For.Light Setting
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	// 로고씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
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

HRESULT CScene_Stage::Ready_LightInfo()
{
	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	D3DLIGHT9				LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

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
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// For.Component_Shader_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;

	// For.Component_Shader_Sky 
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Sky", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;

	// For.Component_Shader_Mesh
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;

	// For.Component_Texture_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga", 2))))
		return E_FAIL;

	// For.Component_Texture_Filter
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Filter", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Filter.bmp", 1))))
		return E_FAIL;

	// For.Component_Texture_Brush
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Brush", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Brush.png", 1))))
		return E_FAIL;

	// For.Component_Texture_Monster
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Monster", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Monster/Stand/AKIHA_AKI00_00%d.png", 12))))
		return E_FAIL;

	// For.Component_Texture_Effect
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Texture_Effect", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Explosion/Explosion%d.png", 90))))
		return E_FAIL;

	// For.Component_Buffer_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Terrain/Height.bmp"))))
		return E_FAIL;

	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Mesh_TombStone
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_TombStone", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/TombStone/", L"TombStone.x"))))
		return E_FAIL;

	// For.Component_Mesh_Tiger
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Tiger", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/Tiger/", L"Tiger.x"))))
		return E_FAIL;

	// For.Component_Mesh_Player
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Player", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Hermione/", L"Hermione.x"))))
		return E_FAIL;

	// For.Component_Mesh_Weapon
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Weapon", CMesh_Static::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/StaticMesh/Wand/", L"Wand.x"))))
		return E_FAIL;

	// For.Component_Mesh_Monster
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STAGE, L"Component_Mesh_Monster", CMesh_Dynamic::Create(m_pGraphic_Device, L"../Bin/Resources/Meshes/DynamicMesh/Juliet/", L"Player.x"))))
		return E_FAIL;


	return NOERROR;
}

HRESULT CScene_Stage::Ready_GameObject_Prototype()
{
	// For.GameObject_SkyBox
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_UI
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_UI", CUI::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Player
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Player", CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Weapon
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Weapon", CWeapon::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Monster
	if (FAILED(Add_Object_Prototype(SCENE_STAGE, L"GameObject_Monster", CMonster::Create(m_pGraphic_Device))))
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
	// For.Camera
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Camera_Dynamic", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0.f, 7.f, -10.f), _vec3(0.f, 0.f, 0.f), _vec3(0.0f, 1.f, 0.f), D3DXToRadian(60.0f), _float(g_iWinCX) / g_iWinCY, 0.2f, 500.f))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.Terrain
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// For.SkyBox
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_SkyBox", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	for (size_t i = 0; i < 3; ++i)
	{
		// For.Monster
		if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_Monster", SCENE_STAGE, pLayerTag)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	// For.UI
	if (FAILED(Add_Object(SCENE_STAGE, L"GameObject_UI", SCENE_STAGE, pLayerTag)))
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
	CScene::Free();
}


