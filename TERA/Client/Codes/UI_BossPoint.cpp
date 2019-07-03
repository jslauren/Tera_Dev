#include "stdafx.h"
#include "..\Headers\UI_BossPoint.h"
#include "Management.h"
#include "Camera_Dynamic.h"
#include "Arkus.h"
#include "FontManager.h"

_USING(Client)

CUI_BossPoint::CUI_BossPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	ZeroMemory(m_szHP, sizeof(_tchar) * 64);
}

CUI_BossPoint::CUI_BossPoint(const CUI_BossPoint & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BossPoint::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_BossPoint::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformPointBoardCom->Set_Scaling((g_iWinCX * 1.f), (g_iWinCY * 0.12f), 0.f);
	m_pTransformPointBoardCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.42f), 0.f));

	m_pTransformHpCom->Set_Scaling((g_iWinCX * 0.515f), (g_iWinCY * 0.12f), 0.f);
	m_pTransformHpCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.01f), (g_iWinCY * 0.418f), 0.f));

	return NOERROR;
}

_int CUI_BossPoint::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	CutSceneEvent();

	return _int();
}

_int CUI_BossPoint::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_BossPoint::Render_GameObject()
{
	if (CManagement::GetInstance()->Get_CurrentScene() != SCENE_LOADING)
	{
		if (m_bIsHpRender == true)
		{
			if (FAILED(NullCheck()))
				return E_FAIL;

			LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
			if (nullptr == pEffect)
				return E_FAIL;

			pEffect->AddRef();


			if (FAILED(SetUp_ConstantTable(pEffect)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(0);

			m_pBufferPointBoardCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();

			{
				if (FAILED(SetUp_ConstantTable(pEffect, 2)))
					return E_FAIL;

				pEffect->Begin(nullptr, 0);
				pEffect->BeginPass(3);

				m_pBufferHpCom->Render_Buffer();

				pEffect->EndPass();
				pEffect->End();
			}

			Safe_Release(pEffect);

			CArkus* pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

			wsprintf(m_szHP, L"%d / %d", (_int)pArkus->Get_HP(), (_int)m_fHP);
			CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.43f), (g_iWinCY * 0.0825f), 0.f), m_szHP);
		}
	}

	return NOERROR;
}

HRESULT CUI_BossPoint::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformPointBoard", (CComponent**)&m_pTransformPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBossHp", (CComponent**)&m_pTransformHpCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Point_Boss_Board", L"Com_BufferPoint_Boss_Board", (CComponent**)&m_pBufferPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Boss_Hp", L"Com_BufferBossHpFilter", (CComponent**)&m_pBufferHpCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Point_Boss_Board", L"Com_TexturePoint_Boss_Board", (CComponent**)&m_pTexturePointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Boss_Hp", L"Com_TextureBossHp", (CComponent**)&m_pTextureHpCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_BossPoint::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	CArkus* pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

	pEffect->AddRef();

	_matrix		matTmp, matProj;
	D3DXMatrixIdentity(&matTmp);

	// UI는 직교투영을 해야하기 때문에 이렇게 처리해준다.
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixOrthoLH(&matProj, g_iWinCX, g_iWinCY, 0.f, 1.f);

	if (1 == iTargetTextureIdx)
	{
		m_pTexturePointBoardCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformPointBoardCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (2 == iTargetTextureIdx)
	{
		m_pTextureHpCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformHpCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		if (m_bIsCutSceneStart == false)
			PointCalculater(pArkus->Get_HP());

		pEffect->SetFloat("g_fBossHPValue", (m_fHPRatio));
	}

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_BossPoint::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pTransformPointBoardCom)
		return E_FAIL;

	if (nullptr == m_pBufferPointBoardCom)
		return E_FAIL;

	if (nullptr == m_pTexturePointBoardCom)
		return E_FAIL;

	if (nullptr == m_pTransformHpCom)
		return E_FAIL;

	if (nullptr == m_pBufferHpCom)
		return E_FAIL;

	if (nullptr == m_pTextureHpCom)
		return E_FAIL;

	return NOERROR;
}

void CUI_BossPoint::CutSceneEvent()
{
	if (m_bIsCutSceneStart == true)
	{
		m_bIsHpRender = true;

		m_fHPRatio += (m_fTimeDelta * 0.35);

		if (m_fHPRatio >= 1)
			m_bIsCutSceneStart = false;
	}
}

void CUI_BossPoint::PointCalculater(_float fCurrentValue)
{
	if (fCurrentValue == 100000.f)
	{
		m_fHPRatio = 1.f;
		return;
	}
	else if (fCurrentValue <= 0.f)
		return;

	m_fCalculatedHP = 100 * (m_fHP - fCurrentValue) / 100000.f * 0.01;
	m_fHPRatio = 1 - m_fCalculatedHP;

}

CUI_BossPoint * CUI_BossPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_BossPoint* pInstance = new CUI_BossPoint(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_BossPoint Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_BossPoint::Clone(void * pArg)
{
	CUI_BossPoint* pInstance = new CUI_BossPoint(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_BossPoint Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossPoint::Free()
{
	Safe_Release(m_pTransformPointBoardCom);
	Safe_Release(m_pBufferPointBoardCom);
	Safe_Release(m_pTexturePointBoardCom);

	Safe_Release(m_pTransformHpCom);
	Safe_Release(m_pBufferHpCom);
	Safe_Release(m_pTextureHpCom);

	CUI::Free();
}
