#include "..\Headers\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera::Ready_GameObject()
{
	return NOERROR;
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CCamera::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CCamera::Render_GameObject()
{
	return NOERROR;
}

void CCamera::Free()
{
	CGameObject::Free();
}
