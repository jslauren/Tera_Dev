#include "stdafx.h"
#include "..\Headers\UI_DamageFont_Manager.h"
#include "UI_DamageTexture.h"

_USING(Client)

_IMPLEMENT_SINGLETON(CUI_DamageFont_Manager)

CUI_DamageFont_Manager::CUI_DamageFont_Manager()
{
}

HRESULT CUI_DamageFont_Manager::Create_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 & vPos, const float & fDamageValue)
{
	_vec3 vInfoV = vPos;

	_vec3 vOut;

	_matrix	matWorld, matView, matProj;
	D3DVIEWPORT9 viewPort;

	ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));

	pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matView);
	pGraphic_Device->GetViewport(&viewPort);

	D3DXMatrixIdentity(&matWorld);

	D3DXVec3Project(&vOut, &vInfoV, &viewPort, &matProj, &matView, &matWorld);

	vOut.x -= (g_iWinCX / 2);
	vOut.y *= -1;
	vOut.y += (g_iWinCY / 2);

	CUI_DamageTexture*	pDamageTexture = CUI_DamageTexture::Create(pGraphic_Device, &_vec4(vOut, fDamageValue));
	if (nullptr == pDamageTexture)
		return E_FAIL;

	m_DamageFontList.push_back(pDamageTexture);
}

void CUI_DamageFont_Manager::Update_DamageFont(const float & fTimeDelta)
{
	auto iter = m_DamageFontList.begin();
	auto iter_end = m_DamageFontList.end();

	for (; iter != iter_end;)
	{
		int iDeleteInfo = (*iter)->Update_GameObject(fTimeDelta);

		if (iDeleteInfo == 1)
		{
			Engine::Safe_Release(*iter);
			iter = m_DamageFontList.erase(iter);
		}
		else
			++iter;
	}
}

void CUI_DamageFont_Manager::Render_DamageFont()
{
	for (auto& iter : m_DamageFontList)
		iter->Render_GameObject();
}

void CUI_DamageFont_Manager::Free()
{
	for (auto& iter : m_DamageFontList)
		Safe_Release(iter);

	m_DamageFontList.clear();
}
