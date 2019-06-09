#include "stdafx.h"
#include "..\Headers\SceneStatic.h"
#include "Camera_Dynamic.h"
#include "SkyBox.h"
#include "Input_device.h"
#include "Terrain.h"
#include "EventManager.h"
#include "Buffer_Terrain_Tool.h"
#include "Texture_Tool.h"
#include "Light_Manager.h"
#include "StaticObject.h"
#include "DataManager.h"

CSceneStatic::CSceneStatic(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
	, m_pViewManager(CViewManager::GetInstance())
{
	m_pViewManager->AddRef();
	//m_pEventManager->m_pGraphic_Device = _pGraphic_Device;
}

HRESULT CSceneStatic::Ready_Scene()
{
	// For.Light Setting
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

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
	m_pViewManager->m_pEditorView->m_Tab_Mesh.Render_Navigation();

	return CScene::Render_Scene();
}

HRESULT CSceneStatic::Add_Static_Object(const _tchar * pLayerTag)
 {
	// For.Static_Mesh
	if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Static", SCENE_STATIC, pLayerTag)))
		return E_FAIL;
}

HRESULT CSceneStatic::Add_Static_Object_Component_Prototype(const _tchar * pComponentPrototypeTag, const _tchar * pFilePath, const _tchar * pFileName)
{
	// For.Component_Static_Object
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, pComponentPrototypeTag, CMesh_Static::Create(m_pGraphic_Device, pFilePath, pFileName))))
		return E_FAIL;
}

HRESULT CSceneStatic::Ready_LightInfo()
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
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pLight_Manager->Add_Light(m_pGraphic_Device, &LightInfo)))
		return E_FAIL;

	Safe_Release(pLight_Manager);

	////((L.Specular * M.Specular) * ½ºÆåÅ§·¯ÀÇ ¼¼±â) + (L.Diffuse * M.Diffuse) * ¸í¾Ï + (L.Ambient * M.Ambient)
	//m_pGraphic_Device->SetLight(0, &LightInfo);
	//m_pGraphic_Device->LightEnable(0, TRUE);

	return NOERROR;
}

HRESULT CSceneStatic::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// For.Component_Shader_Sky 
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Sky", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Sky.fx"))))
		return E_FAIL;

	// For.Component_Shader_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Terrain", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Terrain.fx"))))
		return E_FAIL;

	// For.Component_Shader_Mesh
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Mesh", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Mesh.fx"))))
		return E_FAIL;

	//// For.Component_Shader_StaticObject
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_StaticObject", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_StaticObject.fx"))))
	//	return E_FAIL;

	// For.Component_Buffer_CubeBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_CubeBox", CBuffer_CubeTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Texture_SkyBox
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
		return E_FAIL;


	// For.Component_Texture_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Common_BG/Texture2D/CM_Floor_08_Diff.tga"))))
		return E_FAIL;

	_tchar* wsr = L"../Bin/Resources/Textures/Terrain/Common_BG/Texture2D/CM_Floor_08_Diff.tga";

	CDataManager::GetInstance()->m_pImgPath = wsr;

	//CopyMemory(CDataManager::GetInstance()->m_pImgPath, wsr, sizeof(_tchar)*(lstrlen(wsr) + 1));

	// For.Component_Buffer_Terrain
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_Terrain", CBuffer_Terrain_Tool::Create(m_pGraphic_Device))))
		return E_FAIL;



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

	// For.GameObject_Static
	if (FAILED(Add_Object_Prototype(SCENE_STATIC, L"GameObject_Static", CStaticObject::Create(m_pGraphic_Device))))
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
	Safe_Release(m_pViewManager);

	if (nullptr == m_pObject_Manager)
		return;

	//if (FAILED(m_pObject_Manager->Clear_Object(SCENE_STATIC)))
	//	return;

	CScene::Free();
}
