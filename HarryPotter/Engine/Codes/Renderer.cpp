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
	for (size_t i = 0; i < RENDER_END; i++)
	{
		for (auto& pGameObject : m_RenderList[i])
		{
			if (nullptr != pGameObject)
				pGameObject->Render_GameObject();

			Safe_Release(pGameObject);
		}
		m_RenderList[i].clear();
	}

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
