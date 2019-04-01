#include "..\Headers\Management.h"
#include "Scene.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"

_IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

HRESULT CManagement::SetUp_CurrentScene(CScene * pScene)
{
	if (nullptr == pScene)
		return E_FAIL;

	if (nullptr != m_pCurrentScene)
	{
		if (0 != Safe_Release(m_pCurrentScene))
			_MSGBOX("Do not Deleted CurrentScene");
	}

	m_pCurrentScene = pScene;

	m_pCurrentScene->AddRef();

	return NOERROR;
}

HRESULT CManagement::Ready_Management(const _uint& iMaxNumScene)
{
	if (FAILED(CObject_Manager::GetInstance()->Reserve_Object_Manager(iMaxNumScene)))
		return E_FAIL;

	return NOERROR;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	_int		iExitCode = 0;

	iExitCode = m_pCurrentScene->Update_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return -1;

	iExitCode = m_pCurrentScene->LateUpdate_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return -1;

	return _int();
}

HRESULT CManagement::Render_Management()
{
	if (nullptr == m_pCurrentScene)
		return E_FAIL;

	if (FAILED(m_pCurrentScene->Render_Scene()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CManagement::Release_Engine()
{
	if (0 != CManagement::GetInstance()->DestroyInstance())
		_MSGBOX("CManagement Release Failed");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		_MSGBOX("CObject_Manager Release Failed");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		_MSGBOX("CTimer_Manager Release Failed");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		_MSGBOX("CGraphic_Device Release Failed");

	return NOERROR;
}

void CManagement::Free()
{
	Safe_Release(m_pCurrentScene);
}
