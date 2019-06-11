#pragma once
#include "Component.h"

_BEGIN(Engine)

class CTransform;
class _DLL_EXPORTS CFrustum final : public CComponent
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum() = default;
public:
	HRESULT	Ready_Frustum();
	_bool	isIn_Frustum(const _vec3* pWorldPoint, const CTransform* pTransform, _float fRadius = 0.f);
private:
	_vec3	m_vPoint[8];
private:
	HRESULT Transform_ToLocal(const CTransform* pTransform, _vec3* pPointArray);
	HRESULT Make_Plane(const _vec3* pPoints, D3DXPLANE* pPlane);
public:
	static CFrustum*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();

};

_END