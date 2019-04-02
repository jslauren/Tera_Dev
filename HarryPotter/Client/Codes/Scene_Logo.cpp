#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Back_Logo.h"

_USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Logo::Ready_Scene()
{
	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	return _int();
}

_int CScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CScene_Logo::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene_Logo::Ready_GameObject_Prototype()
{
	// For.GameObject_BackLogo
	// Engine Project의 Scene Class의 Add_Object_Prototype함수 호출
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_BackLogo", CBack_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Logo::Free()
{
	CScene::Free();
}


