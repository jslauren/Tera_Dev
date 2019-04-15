#pragma once

#include "Component.h"

// 객체의 상태를 표현하기위한 클래스이다.
// ex : 위치, 크기, 회전.

_BEGIN(Engine)

class _DLL_EXPORTS CTransform final : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	HRESULT SetUp_OnGraphicDev();
	HRESULT	Scaling(const _float& fX, const _float& fY, const _float& fZ);
public:
	HRESULT Ready_Transform();
private:
	_matrix		m_matWorld;
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END