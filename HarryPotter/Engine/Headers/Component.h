#pragma once
#include "Base.h"

// �پ��� ������Ʈ Ŭ�������� �θ� �Ǵ� Ŭ����.
// ���� ��ü���� �� Ŭ������ �ڽ�Ŭ�������� �������� ����, �ؽ���, Ʈ������ ��ü(Ŭ����)����,
// ����Լ��� ������ �ش� ��ɵ��� ����� �� �ְ� �ȴ�.

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
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	_bool				m_isClone = false;
public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void		Free();
};

_END