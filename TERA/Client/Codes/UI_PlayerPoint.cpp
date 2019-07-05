#include "stdafx.h"
#include "..\Headers\UI_PlayerPoint.h"
#include "Management.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "FontManager.h"

_USING(Client)

CUI_PlayerPoint::CUI_PlayerPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	ZeroMemory(m_szHP, sizeof(_tchar) * 64);
}

CUI_PlayerPoint::CUI_PlayerPoint(const CUI_PlayerPoint & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerPoint::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_PlayerPoint::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformPointBoardCom->Set_Scaling((g_iWinCX * 0.68f), (g_iWinCY * 0.15f), 0.f);
	m_pTransformPointBoardCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.f), -(g_iWinCY * 0.305f), 0.f));

	m_pTransformHpCom->Set_Scaling((g_iWinCX * 0.18f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformHpCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.115f), -(g_iWinCY * 0.305f), 0.f));

	// (g_iWinCX * 0.34f)
	m_pTransformMpCom->Set_Scaling((g_iWinCX * 0.18f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformMpCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.105f), -(g_iWinCY * 0.305f), 0.f));

	return NOERROR;
}

_int CUI_PlayerPoint::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	return _int();
}

_int CUI_PlayerPoint::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_PlayerPoint::Render_GameObject()
{
	if (CManagement::GetInstance()->Get_CurrentScene() != SCENE_LOADING)
	{
		// 드래곤 씬에서 다이나믹 카메라로 방송중일때는 렌더하지 않는다.
		if (CManagement::GetInstance()->Get_CurrentScene() == SCENE_DRAGON &&
			dynamic_cast<CCamera_Dynamic*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera"))->Get_PlayDynamicCamInfo() == true)
		{
			return NOERROR;
		}
		
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
			pEffect->BeginPass(4);

			m_pBufferHpCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

		{
			if (FAILED(SetUp_ConstantTable(pEffect, 3)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(5);

			m_pBufferMpCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

		Safe_Release(pEffect);

		// Font 렌더 부분 //
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.295f), (g_iWinCY * 0.775f), 0.f), L"Lv.30 빡정수");

		wsprintf(m_szHP, L"%d / %d", (_int)pPlayer->Get_HP(), (_int)m_fHP);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.34f), (g_iWinCY * 0.8f), 0.f), m_szHP);

		wsprintf(m_szHP, L"%d \%%", (_int)(m_fMPRatio * 100));
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.585f), (g_iWinCY * 0.8f), 0.f), m_szHP);

		////////////////////
	}

	return NOERROR;
}

HRESULT CUI_PlayerPoint::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformPointBoard", (CComponent**)&m_pTransformPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformHpFilter", (CComponent**)&m_pTransformHpCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformMpFilter", (CComponent**)&m_pTransformMpCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Point_Board", L"Com_BufferPoint_Board", (CComponent**)&m_pBufferPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Hp", L"Com_BufferHpFilter", (CComponent**)&m_pBufferHpCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Mp", L"Com_BufferMpFilter", (CComponent**)&m_pBufferMpCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Point_Board", L"Com_TexturePoint_Board", (CComponent**)&m_pTexturePointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Hp", L"Com_TextureHpFilter", (CComponent**)&m_pTextureHpCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Mp", L"Com_TextureMpFilter", (CComponent**)&m_pTextureMpCom)))
		return E_FAIL;
	
	return NOERROR;
}

HRESULT CUI_PlayerPoint::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

	pEffect->AddRef();

	_matrix		matTmp, matProj;
	D3DXMatrixIdentity(&matTmp);

	// UI는 직교투영을 해야하기 때문에 이렇게 처리해준다.
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixOrthoLH(&matProj, (_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f);

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

 		PointCalculater(true, pPlayer->Get_HP());
		pEffect->SetFloat("g_fPlayerHpValue", (m_fHPRatio));
	}
	else if (3 == iTargetTextureIdx)
	{
		m_pTextureMpCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformMpCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		PointCalculater(false, pPlayer->Get_MP());
		pEffect->SetFloat("g_fPlayerMpValue", (m_fMPRatio));
		
	}
	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_PlayerPoint::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	//if (nullptr == m_pTextureCom)
	//	return E_FAIL;

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

	if (nullptr == m_pTransformMpCom)
		return E_FAIL;

	if (nullptr == m_pBufferMpCom)
		return E_FAIL;

	if (nullptr == m_pTextureMpCom)
		return E_FAIL;


	return NOERROR;
}

void CUI_PlayerPoint::PointCalculater(_bool bIsHP, _float fCurrentValue)
{
	if (fCurrentValue == 10686.f)
	{
		m_fHPRatio = 0.f;
		return;
	}
	else if (fCurrentValue == 3250.f)
	{
		m_fMPRatio = 1.f;
		return;
	}
	else if (fCurrentValue <= 0.f)
		return;

	if (bIsHP == true)
	{
		m_fCalculatedHP = 100 * (m_fHP - fCurrentValue) / 10686.f * 0.01f;
		m_fHPRatio = /*1 -*/ m_fCalculatedHP;
	}

	else if (bIsHP == false)
	{
		m_fCalculatedMP = 100 * (m_fMP - fCurrentValue) / 3250.f * 0.01f;
		m_fMPRatio = 1 - m_fCalculatedMP;
	}

}

CUI_PlayerPoint * CUI_PlayerPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerPoint* pInstance = new CUI_PlayerPoint(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_PlayerPoint Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_PlayerPoint::Clone(void * pArg)
{
	CUI_PlayerPoint* pInstance = new CUI_PlayerPoint(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_PlayerPoint Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerPoint::Free()
{
	Safe_Release(m_pTransformPointBoardCom);
	Safe_Release(m_pBufferPointBoardCom);
	Safe_Release(m_pTexturePointBoardCom);

	Safe_Release(m_pTransformHpCom);
	Safe_Release(m_pBufferHpCom);
	Safe_Release(m_pTextureHpCom);

	Safe_Release(m_pTransformMpCom);
	Safe_Release(m_pBufferMpCom);
	Safe_Release(m_pTextureMpCom);

	CUI::Free();
}
