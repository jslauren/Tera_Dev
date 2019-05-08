#pragma once
#include "Base.h"
#include "Object_Manager.h"
#include "../Client/Headers/Defines.h"

class CDataManager final : public CBase
{
	_DECLARE_SINGLETON(CDataManager)
private:
	explicit CDataManager();
	virtual ~CDataManager() = default;
private:
	CObject_Manager*		m_pObjectManager = nullptr;
public:
	virtual void Free();
};

