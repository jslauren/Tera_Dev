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

void CCamera::ComputRayInfo(POINT & ptMouse)
{
	_vec3 vMouse = _vec3(float(ptMouse.x), float(ptMouse.y), 0.f);
	ComputRayInfo(vMouse);
}

void CCamera::ComputRayInfo(_vec3 & vMouse)
{
	D3DVIEWPORT9 viewPort;
	m_pGraphic_Device->GetViewport(&viewPort);

	//뷰포트좌표계 -> 투영좌표계
	vMouse.x = vMouse.x * 2.f / viewPort.Width - 1.f;
	vMouse.y = -vMouse.y * 2.f / viewPort.Height + 1.f;
	vMouse.z = 0;

	//투영좌표계 -> 뷰스페이스
	m_tRay.vPos = _vec3(0.f, 0.f, 0.f);
	m_tRay.vDirection = vMouse - m_tRay.vPos;

	_matrix matProj;
	D3DXMatrixInverse(&matProj, 0, &m_matProj);
	D3DXVec3TransformCoord(&m_tRay.vDirection, &m_tRay.vDirection, &matProj);

	//뷰스페이스 -> 월드스페이스
	_matrix matView;
	D3DXMatrixInverse(&matView, 0, &m_matView);
	D3DXVec3TransformCoord(&m_tRay.vPos, &m_tRay.vPos, &matView);
	D3DXVec3TransformNormal(&m_tRay.vDirection, &m_tRay.vDirection, &matView);
}

void CCamera::Free()
{
	CGameObject::Free();
}
