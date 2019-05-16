#pragma once
#include "Base.h"

class CEventManagerTool final : public CBase
{
	_DECLARE_SINGLETON(CEventManagerTool)

private:
	explicit CEventManagerTool();
	virtual ~CEventManagerTool() = default;

public:
	_bool	m_bIsWireFrame = false;

	LPDIRECT3DDEVICE9	m_pGraphic_Device;

public:
	virtual void		Free();

};

