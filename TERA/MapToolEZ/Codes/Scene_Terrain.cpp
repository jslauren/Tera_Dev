#include "stdafx.h"
#include "..\Headers\Scene_Terrain.h"

_USING(MapTool)

CScene_Terrain::CScene_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Terrain::Ready_Scene()
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

	return NOERROR;
}

_int CScene_Terrain::Update_Scene(const _float & fTimeDelta)
{
	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Terrain::LateUpdate_Scene(const _float & fTimeDelta)
{
	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Terrain::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CScene_Terrain::Ready_LightInfo()
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

HRESULT CScene_Terrain::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Terrain::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Terrain::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	return NOERROR;
}

CScene_Terrain * CScene_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Terrain* pInstance = new CScene_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Terrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Terrain::Free()
{
	CScene::Free();
}
