#pragma once

#include "Base.h"

// �پ��� ������Ʈ Ŭ�������� �θ� �Ǵ� Ŭ����.

_BEGIN(Engine)

class _DLL_EXPORTS CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;
public:
	virtual HRESULT Ready_Component();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
public:
	virtual CComponent* Clone() = 0;
	virtual void Free();
};

_END