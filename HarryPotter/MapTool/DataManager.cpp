#include "stdafx.h"
#include "DataManager.h"

_IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
	:m_pObjectManager(CObject_Manager::GetInstance())
{
}

void CDataManager::Free()
{
}
