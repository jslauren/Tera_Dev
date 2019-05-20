#include "stdafx.h"
#include "..\Headers\SceneStatic.h"
#include "Camera_Dynamic.h"
#include "SkyBox.h"
#include "Input_device.h"
#include "Terrain.h"
#include "EventManager.h"

CSceneStatic::CSceneStatic(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
	, m_pEventManager(CEventManagerTool::GetInstance())
{
	m_pEventManager->AddRef();
	//m_pEventManager->m_pGraphic_Device = _pGraphic_Device;
}

HRESULT CSceneStatic::Ready_Scene()
{
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneStatic::Update_Scene(const _float & fTimeDelta)
{
	// For the JunSikkkk !!!!!!!!!!!!!!
	if (FAILED(CInput_Device::GetInstance()->SetUp_Input_State()))
		return -1;

	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneStatic::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneStatic::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneStatic::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// For.Component_Buffer_CubeBox
	// SkyBox를 위한 버퍼
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;

	// For.Component_Buffer_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(m_pGraphic_Device/*, L"../Bin/Resources/Textures/Terrain/Height.bmp"*/))))
		return E_FAIL;

	//// For.Component_Texture_Terrain
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Grass/Grass_0.tga"/*L"../Bin/Resources/Textures/Terrain/Grass/Grass_%d.tga", 2*/))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CSceneStatic::Ready_GameObject_Prototype()
{
	// For.GameObject_SkyBox
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.GameObject_Terrain
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSceneStatic::Ready_GameObject()
{
	// For.SkyBox
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_SkyBox", SCENE_STATIC, L"Layer_BackGround")))
		return E_FAIL;

	// For.Camera
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Camera_Dynamic", SCENE_STATIC, L"Layer_Camera", &CCamera::CAMERAINFO(_vec3(0.f, 7.f, -10.f), _vec3(0.f, 0.f, 0.f), _vec3(0.0f, 1.f, 0.f), D3DXToRadian(60.0f), _float(g_iWinCX) / g_iWinCY, 0.2f, 500.f))))
		return E_FAIL;

	// For.Terrain
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Terrain", SCENE_STATIC, L"Layer_Terrain")))
		return E_FAIL;

	return NOERROR;
}

CSceneStatic * CSceneStatic::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneStatic* pInstance = new CSceneStatic(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneStatic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneStatic::Free()
{
	Safe_Release(m_pEventManager);

	if (nullptr == m_pObject_Manager)
		return;

	//if (FAILED(m_pObject_Manager->Clear_Object(SCENE_STATIC)))
	//	return;

	CScene::Free();
}
