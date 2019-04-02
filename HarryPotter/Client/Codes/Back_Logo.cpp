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
// ���� ��ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	// ���� ��, ����� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

//���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü ���� �ܿ���, �߰����� ������ �ʿ��ϸ� ���⼭ �����ϸ� �ȴ�.
HRESULT CBack_Logo::Ready_GameObject()
{
	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	return NOERROR;
}

// ���� ��ü�� �����Ѵ�.
CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? :
// 1. ����Լ� �ȿ� �����Ѵ�.
// 2. ����Լ��� ��ü�� ���� ȣ��(��ü.����Լ�(), ��ü�ּ�->����Լ�())
// 3. ����Լ��ȿ� �����ϴ� this�� ����Լ��� ȣ���� �����ϰ� �� ��ü�� �ּҸ� �ǹ��Ѵ�.
CGameObject * CBack_Logo::Clone()
{
	// *this �� ������ü�� �ش� ��.
	// ���� �����ڸ� ȣ���ϸ� �θ��� ���� �����ڵ� ȣ���Ѵ�.
	// ��ġ ��ü ������ ���� ���縦 �ϸ� ������ �����.
	// (������ �����ʹ� ���������ص� ����� ���ٰ� �Ѵ�)
	// AddRefCnt ������ ���� ���� �ϸ� �ȵȴ�.
	CBack_Logo* pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBack_Logo::Free()
{
	CGameObject::Free();
}
