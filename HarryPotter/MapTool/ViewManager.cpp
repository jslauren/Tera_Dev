#include "stdafx.h"
#include "ViewManager.h"
#include "DataManager.h"

CViewManager::CViewManager()
	:m_pManagement(CManagement::GetInstance())
{
}

HRESULT CViewManager::SetCurScene(TOOLID _eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	return NOERROR;
}

void CViewManager::Free()
{
	//Safe_Release(m_pManagement);
}
