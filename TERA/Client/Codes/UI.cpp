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

// ������ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CUI::Ready_GameObject_Prototype()
{
	return NOERROR;
}

// ���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü �����ܿ��� �߰����� �������ʿ��ϸ� ���⼭ �����ض�.
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

	// ���⼭ ���ϰ� �긦 ��ӹ޴� �ڽ� Ŭ�������� AddComponent���� �ؿ� ������ �̸��� �Ը��� ���� �߰�.
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
