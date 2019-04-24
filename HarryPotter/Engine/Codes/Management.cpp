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
	// �޾ƿ� ���� ���������.. ���Ȱ���..?
	if (nullptr == pScene)
		return E_FAIL;

	// �� ��ȯ ��, ���� ���� ��� ���� �ʾҴٸ�, ���� ��� ����..?
	if (nullptr != m_pCurrentScene)
	{
		// 0 �̿��� ���� ���ϵǸ� �� ������ ������.
		if (0 != Safe_Release(m_pCurrentScene))
			_MSGBOX("�������� �ȳ��󰬾�");
	}

	// �޾ƿ� ���� ���� ������ ����(����ȭ)�Ѵ�.
	m_pCurrentScene = pScene;

	//// m_pCurrentScene�� �޾ƿ� �� pScene�� ���������Ƿ� AddRef ī��Ʈ ���� �÷��ش�.
	// �̰� ���ָ� �ȵȴٳ� ������ ����.
	// m_pCurrentScene->AddRef();

	return NOERROR;
}

HRESULT CManagement::Ready_Management(const _uint& iMaxNumScene)
{
	// MainApp Class�� Ready_Default_Setting�Լ��ȿ��� �� �Լ��� ȣ�� �Ѵ�.
	// �� �Ŵ��� �ʱ�ȭ ������ �� �� �ִ�.

	// iMaxNumScene �� ���ڷ� �޾� �Ѱ��־� �� �Ŵ����� �ʱ�ȭ �� ��,
	// ������Ʈ�� ������Ʈ�� ���� �����ϴ� ���� ������ŭ �������ش�.
	if (FAILED(CObject_Manager::GetInstance()->Reserve_Object_Manager(iMaxNumScene)))
		return E_FAIL;
	if (FAILED(CComponent_Manager::GetInstance()->Reserve_Component_Manager(iMaxNumScene)))
		return E_FAIL;

	return NOERROR;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	// ���� ���� Update ������ �� �Ŵ����� ���Ͽ� �����Ѵ�.

	// ���� ���� Nullptr �̸� ���Ȱ���..?
	if (nullptr == m_pCurrentScene)
		return -1;

	// Ȯ�ο� int ���� �ϳ� �����ؼ�
	_int		iExitCode = 0;

	// return ���� E_FAIL �̸� return �ϴ� ����
	// ���� ��(m_pCurrentScene)�� CScene_Logo�� �����Ͽ��� ��,
	// Engine Project�� Scene ������ Scene Class�� Update_Scene �Լ��� �����Ͽ��� ������,
	// Client Project�� Scene ������ Scene_Logo Class�� �ش� Scene Class�� ��� �ް� �����Ƿ�,
	// Client Project�� Scene ������ Scene_Logo Class�� Upadate_Scene �Լ��� �����Ѵ�.
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

	// ���� Update_Management �Լ��� ���ٰ� �����ϴ�.
	if (FAILED(m_pCurrentScene->Render_Scene()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CManagement::Release_Engine()
{
	// ��ü���� Release �� ��, ������ ������ �����Ͽ��� �Ѵ�.
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
