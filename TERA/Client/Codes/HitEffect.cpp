#include "stdafx.h"
#include "..\Headers\HitEffect.h"

_USING(Client)

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{
	m_fFrame = 0.f;
}

CHitEffect::CHitEffect(const CEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CHitEffect::Ready_GameObject_Prototype()
{
	CEffect::Ready_GameObject_Prototype();

	return NOERROR;
}

HRESULT CHitEffect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vColliderPos = (*(_vec3*)(pArg));

	m_pTransformCom->Set_Scaling(10.f, 10.f, 1.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(vColliderPos.x, vColliderPos.y, vColliderPos.z));

	return NOERROR;
}

_int CHitEffect::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	SetUp_BillBoard();

	m_fFrame += 32.f * fTimeDelta;

	if (16.f < m_fFrame)
	{
		m_fFrame = 0.f;
		return -1;
	}

	return _int();
}

_int CHitEffect::LateUpdate_GameObject(const _float & fTimeDelta)
{
	CEffect::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CHitEffect::Render_GameObject()
{
	// 렌더함수에서 돌리고 있는 SetUp_BillBoard 함수에서,
	// Set_StateInfo 함수를 써서 트랜스폼 컴포넌트 값이 초기화 되나봐...
	// 다시 잡아주기 용도.
	m_pTransformCom->Set_Scaling(30.f, 30.f, 0.f);

	CEffect::Render_GameObject();

	return NOERROR;
}

HRESULT CHitEffect::Add_Component()
{
	CEffect::Add_Component();

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_HitEffect", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

CHitEffect * CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitEffect* pInstance = new CHitEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CHitEffect Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHitEffect::Clone(void * pArg)
{
	CHitEffect* pInstance = new CHitEffect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CHitEffect Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitEffect::Free()
{
	CEffect::Free();
}
