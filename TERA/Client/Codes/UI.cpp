#include "stdafx.h"
#include "..\Headers\UI.h"

_USING(Client)

CUI::CUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CUI::Ready_GameObject_Prototype()
{
	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CUI::Ready_GameObject(void* pArg)
{
	return NOERROR;
}

_int CUI::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CUI::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CUI::Render_GameObject()
{
	return NOERROR;
}

HRESULT CUI::OnEvent(const _tchar * _szEventTag, void * _pMsg)
{
	return NOERROR;
}

HRESULT CUI::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_UI", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// 여기서 안하고 얘를 상속받는 자식 클래스에서 AddComponent에서 밑에 구문을 이름을 입맛에 따라 추가.
	//// For.Com_Texture
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	return NOERROR;
}

void CUI::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
//	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
