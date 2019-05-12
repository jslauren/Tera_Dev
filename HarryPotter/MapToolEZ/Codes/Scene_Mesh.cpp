#include "stdafx.h"
#include "..\Headers\Scene_Mesh.h"

_USING(MapTool)

CScene_Mesh::CScene_Mesh(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Mesh::Ready_Scene()
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

	//// For.Layer_Camera
	//if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
	//	return E_FAIL;
}

_int CScene_Mesh::Update_Scene(const _float & fTimeDelta)
{
	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Mesh::LateUpdate_Scene(const _float & fTimeDelta)
{
	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Mesh::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CScene_Mesh::Ready_LightInfo()
{
	D3DLIGHT9				LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	//((L.Specular * M.Specular) * 스펙큘러의 세기) + (L.Diffuse * M.Diffuse) * 명암 + (L.Ambient * M.Ambient)

	m_pGraphic_Device->SetLight(0, &LightInfo);

	m_pGraphic_Device->LightEnable(0, TRUE);

	return NOERROR;
}

HRESULT CScene_Mesh::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	//// For.Component_Texture_SkyBox
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_PROTO, L"Component_Texture_SkyBox", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/SkyBox/Burger%d.dds", 4))))
	//	return E_FAIL;

	//// For.Component_Texture_Terrain
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_PROTO, L"Component_Texture_Terrain", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Terrain/Grass_%d.tga", 2))))
	//	return E_FAIL;

	//// For.Component_Buffer_Terrain
	//if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_PROTO, L"Component_Buffer_Terrain", CBuffer_Terrain::Create(m_pGraphic_Device, L"../Bin/Resources/Textures/Terrain/Height.bmp"))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Mesh::Ready_GameObject_Prototype()
{
	//// For.GameObject_SkyBox
	//if (FAILED(Add_Object_Prototype(SCENE_PROTO, L"GameObject_SkyBox", CSkyBox::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	//// For.GameObject_Terrain
	//if (FAILED(Add_Object_Prototype(SCENE_PROTO, L"GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Mesh::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	//// For.Camera
	//if (FAILED(Add_Object(SCENE_STATIC, L"GameObject_Camera_Dynamic", SCENE_STAGE, pLayerTag, &CCamera::CAMERAINFO(_vec3(0.f, 7.f, -10.f), _vec3(0.f, 0.f, 0.f), _vec3(0.0f, 1.f, 0.f), D3DXToRadian(60.0f), _float(g_iWinCX) / g_iWinCY, 0.2f, 500.f))))
	//	return E_FAIL;

	return NOERROR;
}

CScene_Mesh * CScene_Mesh::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Mesh* pInstance = new CScene_Mesh(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Mesh Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Mesh::Free()
{
	CScene::Free();
}
