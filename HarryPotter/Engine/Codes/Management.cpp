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
	// 받아온 씬이 비어있으면.. ㅈ된거쥬..?
	if (nullptr == pScene)
		return E_FAIL;

	// 씬 전환 시, 현재 씬이 비어 있지 않았다면, 지워 줘야 겠쥬..?
	if (nullptr != m_pCurrentScene)
	{
		// 0 이외의 값이 리턴되면 잘 안지워 진거쥬.
		if (0 != Safe_Release(m_pCurrentScene))
			_MSGBOX("Do not Deleted CurrentScene");
	}

	// 받아온 씬을 현재 씬으로 셋팅(동기화)한다.
	m_pCurrentScene = pScene;

	// m_pCurrentScene이 받아온 씬 pScene을 참조했으므로 AddRef 카운트 값을 올려준다.
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
	// 객체들을 Release 할 때, 순서에 굉장히 유의하여야 한다.
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
