#pragma once

#include "Component.h"

// ��ü�� ���¸� ǥ���ϱ����� Ŭ�����̴�.
// ex : ��ġ, ũ��, ȸ��.

_BEGIN(Engine)

class _DLL_EXPORTS CTransform final : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTransform() = default;
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