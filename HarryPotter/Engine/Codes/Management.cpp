#include "..\Headers\Management.h"
#include "Scene.h"
#include "Timer_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Component_Manager.h"

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
			_MSGBOX("기존씬이 안날라갔어");
	}

	// 받아온 씬을 현재 씬으로 셋팅(동기화)한다.
	m_pCurrentScene = pScene;

	//// m_pCurrentScene이 받아온 씬 pScene을 참조했으므로 AddRef 카운트 값을 올려준다.
	// 이거 해주면 안된다네 터진데 빵빵.
	// m_pCurrentScene->AddRef();

	return NOERROR;
}

HRESULT CManagement::Ready_Management(const _uint& iMaxNumScene)
{
	// MainApp Class의 Ready_Default_Setting함수안에서 이 함수를 호출 한다.
	// 씬 매니저 초기화 정도로 볼 수 있다.

	// iMaxNumScene 을 인자로 받아 넘겨주어 씬 매니저를 초기화 할 때,
	// 오브젝트나 컴포넌트를 현재 존재하는 씬의 갯수만큼 생성해준다.
	if (FAILED(CObject_Manager::GetInstance()->Reserve_Object_Manager(iMaxNumScene)))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Reserve_Component_Manager(iMaxNumScene)))
		return E_FAIL;

	return NOERROR;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	// 현재 씬의 Update 과정을 씬 매니저를 통하여 진행한다.

	// 현재 씬이 Nullptr 이면 ㅈ된거쥬..?
	if (nullptr == m_pCurrentScene)
		return -1;

	// 확인용 int 변수 하나 생성해서
	_int		iExitCode = 0;

	// return 값이 E_FAIL 이면 return 하는 구문
	// 현재 씬(m_pCurrentScene)이 CScene_Logo라 가정하였을 때,
	// Engine Project의 Scene 필터의 Scene Class의 Update_Scene 함수를 실행하여아 하지만,
	// Client Project의 Scene 필터의 Scene_Logo Class가 해당 Scene Class를 상속 받고 있으므로,
	// Client Project의 Scene 필터의 Scene_Logo Class의 Upadate_Scene 함수를 실행한다.
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

	// 위의 Update_Management 함수의 접근과 동일하다.
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

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		_MSGBOX("CComponent_Manager Release Failed");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		_MSGBOX("CTimer_Manager Release Failed");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		_MSGBOX("CGraphic_Device Release Failed");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		_MSGBOX("CInput_Device Release Failed");

	return NOERROR;
}

void CManagement::Free()
{
	Safe_Release(m_pCurrentScene);
}
