#include "stdafx.h"
#include "..\Headers\Back_Logo.h"

_USING(Client)

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CBack_Logo::CBack_Logo(const CBack_Logo & rhs)
	: CGameObject(rhs)
{
}

// ������ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	// ����������� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// ���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü �����ܿ��� �߰����� �������ʿ��ϸ� ���⼭ �����ض�.
HRESULT CBack_Logo::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(2.f, 2.f, 2.f);
	//m_pTransformCom->Set_Angle_Y(D3DXToRadian(45.0f));

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	//if (GetKeyState('W') & 0x8000)
	//{
	//	m_pTransformCom->Go_Straight(5.f, fTimeDelta);
	//}

	//if (GetKeyState('S') & 0x8000)
	//{
	//	m_pTransformCom->Back_Straight(5.f, fTimeDelta);
	//}

	//if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_X(D3DXToRadian(90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_DOWN) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_X(D3DXToRadian(-90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_LEFT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_Y(D3DXToRadian(-90.f), fTimeDelta);
	//}

	//if (GetKeyState(VK_RIGHT) & 0x8000)
	//{
	//	m_pTransformCom->Rotation_Y(D3DXToRadian(90.f), fTimeDelta);
	//}

	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->SetUp_OnGraphicDev(0);

	// ��� = ��� * ���
	m_pBufferCom->Render_Buffer(m_pTransformCom);

	return NOERROR;
}

HRESULT CBack_Logo::Add_Component()
{
	// ����� ������Ʈ ��ü���� �� �Լ����� �߰����ش�.

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_RcTex", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

// ������ü�� �����Ѵ�.
CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.����Լ��ȿ� ����. 
// 2.����Լ��� ��ü�κ��� ȣ��(��ü.����Լ�(), ��ü�ּ�->����Լ�())
// 3.����Լ��ȿ� �����ϴ� this�� ����Լ��� ȣ���� �����ϰ��� ��ü�� �ּҸ� �ǹ��Ѵ�.
CGameObject * CBack_Logo::Clone(void* pArg)
{
	// *this �� ������ü�� �ش� ��.
	// ���� �����ڸ� ȣ���ϸ� �θ��� ���� �����ڵ� ȣ���Ѵ�.
	// ��ġ ��ü ������ ���� ���縦 �ϸ� ������ �����.
	// (������ �����ʹ� ���������ص� ����� ���ٰ� �Ѵ�)
	// AddRefCnt ������ ���� ���� �ϸ� �ȵȴ�.
	CBack_Logo* pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Logo::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
