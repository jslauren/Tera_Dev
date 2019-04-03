#pragma once
#include "Base.h"
// �پ��� ������Ʈ Ŭ�������� �θ� �Ǵ� Ŭ����.

_BEGIN(Engine)

class _DLL_EXPORTS CComponent : public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CComponent() = default;
public:
	virtual HRESULT Ready_Component();
protected:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
public:
	virtual void Free();
};

_END