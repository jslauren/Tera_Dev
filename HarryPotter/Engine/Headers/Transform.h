#pragma once

#include "Component.h"

// 객체의 상태를 표현하기위한 클래스이다.
// ex : 위치, 크기, 회전.

_BEGIN(Engine)

class _DLL_EXPORTS CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:	// Getter
	// 왠만하면 retrun 하는 Getter는 const를 붙여주자.
	const _matrix*	Get_WorldMatrixPointer() const { return &m_matWorld; }
	const _vec3*	Get_StateInfo(STATE eState) const { return (_vec3*)&m_matWorld.m[eState][0]; }
public:	// Setter
	void			Set_StateInfo(STATE eState, const _vec3* pStateInfo) const {
		memcpy((_vec3*)&m_matWorld.m[eState][0], pStateInfo, sizeof(_vec3));
	}
public:
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT	Set_Scaling(const _float& fX, const _float& fY, const _float& fZ);
	// vState에 right, up, look 벡터로 어떤 축을 기준으로 돌릴껀지 넣어준다.
	HRESULT	Set_Angle_Axis(_vec3 vState, const _float& fRadian);
	// iDirection에 0이면 Go, 1이면 Back 이다.
	HRESULT	Move(_int& iDirection, const _float& fSpeedPerSec, const _float& fTimeDelta);
	HRESULT	Rotation_Axis(_vec3 vState, const _float& fRadianPerSec, const _float& fTimeDelta);
	HRESULT Move_Target(const CTransform* pTransform, const _float& fSpeedPerSec, const _float& fTimeDelta);
public:
	HRESULT Ready_Transform();
private:
	_matrix	m_matWorld;
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END