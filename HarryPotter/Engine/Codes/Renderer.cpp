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

	// 클라이언트의 GameObject필터에 해당하는 클래스들에서 요청 한 함수로써,
	// 해당 오브젝트들을 먼저 그릴껀지, 나중에 그릴껀지, 알파값은 어떤지, UI로 사용할껀지 등의 정보를 포함하여,
	// 렌더 그룹을 통하여 같은 그룹끼리 일괄적으로 관리하기 위하여 해당 렌더 리스트 컨테이너를 사용한다.

	// 이 렌더 리스트 컨테이너에 추가되는 요소들은 각 그룹에 해당하는 오브젝트들이다.
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
