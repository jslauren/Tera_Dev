#include "stdafx.h"
#include "..\Headers\Scene_Stage.h"

_USING(Client)

CScene_Stage::CScene_Stage(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Stage::Ready_Scene()
{
	// 로고씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
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

HRESULT CScene_Stage::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Stage::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CScene_Stage::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
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
