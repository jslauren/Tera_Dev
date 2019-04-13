#pragma once

#include "Base.h"

// 다양한 컴포넌트 클래스들의 부모가 되는 클래스.

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