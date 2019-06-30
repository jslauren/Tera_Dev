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
	HRESULT Ready_Frustum();

	// 월드영역상에 존재하는 객체의 위치기준으로 컬링.(월드영역)
	_bool WorldPt_InFrustum(const _vec3* pWorldPoint, const CTransform* pTransform, _float fRadius = 0.f);

	// 정정버퍼상에 존재하는 정점의 위치기준으로 컬링.(로컬영역)
	_bool LocalPt_InFrustum(const _vec3* pLocal, const CTransform* pTransform, _float fRadius = 0.f);
	CFrustum* Make_LocalPlane(const CTransform* pTransform);
private:
	_vec3			m_vPoint[8];
	D3DXPLANE		m_Plane[6];
private:
	HRESULT Transform_ToLocal(const CTransform* pTransform, _vec3* pPointArray);
	HRESULT Make_Plane(const _vec3* pPoints, D3DXPLANE* pPlane);
public:
	static CFrustum* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();

};

_END