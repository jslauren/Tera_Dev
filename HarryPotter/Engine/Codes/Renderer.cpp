#include "..\Headers\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CRenderer::Ready_Renderer()
{
	return NOERROR;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eGroup, CGameObject * pGameObject)
{
	if (RENDER_END <= eGroup ||
		nullptr == pGameObject)
		return E_FAIL;

	// Ŭ���̾�Ʈ�� GameObject���Ϳ� �ش��ϴ� Ŭ�����鿡�� ��û �� �Լ��ν�,
	// �ش� ������Ʈ���� ���� �׸�����, ���߿� �׸�����, ���İ��� ���, UI�� ����Ҳ��� ���� ������ �����Ͽ�,
	// ���� �׷��� ���Ͽ� ���� �׷쳢�� �ϰ������� �����ϱ� ���Ͽ� �ش� ���� ����Ʈ �����̳ʸ� ����Ѵ�.

	// �� ���� ����Ʈ �����̳ʿ� �߰��Ǵ� ��ҵ��� �� �׷쿡 �ش��ϴ� ������Ʈ���̴�.
	m_RenderList[eGroup].push_back(pGameObject);

	pGameObject->AddRef();

	return NOERROR;
}

HRESULT CRenderer::Render_RenderGroup()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NoneAlpha()))
		return E_FAIL;
	if (FAILED(Render_Alpha()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderList[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_PRIORITY].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_NoneAlpha()
{
	for (auto& pGameObject : m_RenderList[RENDER_NONEALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}
	m_RenderList[RENDER_NONEALPHA].clear();

	return NOERROR;
}

_bool Compare_Z(CGameObject* pSour, CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

HRESULT CRenderer::Render_Alpha()
{
	m_RenderList[RENDER_ALPHA].sort(Compare_Z);

	for (auto& pGameObject : m_RenderList[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_ALPHA].clear();

	return NOERROR;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderList[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_GameObject();
		Safe_Release(pGameObject);
	}

	m_RenderList[RENDER_UI].clear();

	return NOERROR;
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRenderer* pInstance = new CRenderer(pGraphic_Device);

	if (FAILED(pInstance->Ready_Renderer()))
	{
		_MSGBOX("CRenderer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone()
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
			Safe_Release(pGameObject);
		m_RenderList[i].clear();
	}
	CComponent::Free();
}