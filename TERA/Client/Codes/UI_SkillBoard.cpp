#include "stdafx.h"
#include "..\Headers\UI_SkillBoard.h"
#include "Management.h"
#include "Camera_Dynamic.h"
#include "FontManager.h"
#include "Player.h"

_USING(Client)

CUI_SkillBoard::CUI_SkillBoard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CUI_SkillBoard::CUI_SkillBoard(const CUI_SkillBoard & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillBoard::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_SkillBoard::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	Init_Filters_Transform();	

	return NOERROR;
}

_int CUI_SkillBoard::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	Renewal_PlayerSkill_CoolTime();

	return _int();
}

_int CUI_SkillBoard::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_SkillBoard::Render_GameObject()
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


		if (FAILED(SetUp_ConstantTable(pEffect, 0)))
			return E_FAIL;

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		m_pBufferSkillBoardCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		{
			if (FAILED(SetUp_ConstantTable(pEffect, 1)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[0] != 1.f)
				m_pBufferSkillFilter1Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 2)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[1] != 1.f)
				m_pBufferSkillFilter2Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 3)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[2] != 1.f)
				m_pBufferSkillFilter3Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 4)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[3] != 1.f)
				m_pBufferSkillFilter4Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 5)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[4] != 1.f)
				m_pBufferSkillFilter5Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 6)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[5] != 1.f)
				m_pBufferSkillFilter6Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 7)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[6] != 1.f)
				m_pBufferSkillFilter7Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 8)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[7] != 1.f)
				m_pBufferSkillFilter8Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}
		{
			if (FAILED(SetUp_ConstantTable(pEffect, 9)))
				return E_FAIL;

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(2);

			if (m_fCalculatedCoolTime[8] != 1.f)
				m_pBufferSkillFilter9Com->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

		Safe_Release(pEffect);
	}

	if (m_fCalculatedCoolTime[0] != 1.f)
	{
		wsprintf(m_szCutHead_CT, L"%d", (_int)m_fCurrentCoolTime[0] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.312f), (g_iWinCY * 0.92f), 0.f), m_szCutHead_CT);
	}
	if (m_fCalculatedCoolTime[1] != 1.f)
	{
		wsprintf(m_szCuttingSlash_CT, L"%d", (_int)m_fCurrentCoolTime[1] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.343f), (g_iWinCY * 0.92f), 0.f), m_szCuttingSlash_CT);
	}
	if (m_fCalculatedCoolTime[2] != 1.f)
	{
		wsprintf(m_szFlatBlade_CT, L"%d", (_int)m_fCurrentCoolTime[2] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.375f), (g_iWinCY * 0.92f), 0.f), m_szFlatBlade_CT);
	}
	if (m_fCalculatedCoolTime[3] != 1.f)
	{
		wsprintf(m_szHandySlash_CT, L"%d", (_int)m_fCurrentCoolTime[3] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.407f), (g_iWinCY * 0.92f), 0.f), m_szHandySlash_CT);
	}
	if (m_fCalculatedCoolTime[4] != 1.f)
	{
		wsprintf(m_szJawBreaker_CT, L"%d", (_int)m_fCurrentCoolTime[4] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.439f), (g_iWinCY * 0.92f), 0.f), m_szJawBreaker_CT);
	}
	if (m_fCalculatedCoolTime[5] != 1.f)
	{
		wsprintf(m_szStingerBlade_CT, L"%d", (_int)m_fCurrentCoolTime[5] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.542f), (g_iWinCY * 0.92f), 0.f), m_szStingerBlade_CT);
	}
	if (m_fCalculatedCoolTime[6] != 1.f)
	{
		wsprintf(m_szRagingStrike_CT, L"%d", (_int)m_fCurrentCoolTime[6] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.574f), (g_iWinCY * 0.92f), 0.f), m_szRagingStrike_CT);
	}
	if (m_fCalculatedCoolTime[7] != 1.f)
	{
		wsprintf(m_szDrawSword_CT, L"%d", (_int)m_fCurrentCoolTime[7] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.606f), (g_iWinCY * 0.92f), 0.f), m_szDrawSword_CT);
	}
	if (m_fCalculatedCoolTime[8] != 1.f)
	{
		wsprintf(m_szTumbling_CT, L"%d", (_int)m_fCurrentCoolTime[8] + 1);
		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.507f), (g_iWinCY * 0.92f), 0.f), m_szTumbling_CT);
	}
	
	return NOERROR;
}

HRESULT CUI_SkillBoard::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_SkillBoard", L"Com_TextureSkill_Board", (CComponent**)&m_pTextureSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_SkillFilter", L"Com_TextureSkill_Filter", (CComponent**)&m_pTextureSkillFilterCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillBoard", (CComponent**)&m_pTransformSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter1", (CComponent**)&m_pTransformSkillFilter1Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter2", (CComponent**)&m_pTransformSkillFilter2Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter3", (CComponent**)&m_pTransformSkillFilter3Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter4", (CComponent**)&m_pTransformSkillFilter4Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter5", (CComponent**)&m_pTransformSkillFilter5Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter6", (CComponent**)&m_pTransformSkillFilter6Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter7", (CComponent**)&m_pTransformSkillFilter7Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter8", (CComponent**)&m_pTransformSkillFilter8Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter9", (CComponent**)&m_pTransformSkillFilter9Com)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillBoard", L"Com_BufferSkill_Board", (CComponent**)&m_pBufferSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter1", (CComponent**)&m_pBufferSkillFilter1Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter2", (CComponent**)&m_pBufferSkillFilter2Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter3", (CComponent**)&m_pBufferSkillFilter3Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter4", (CComponent**)&m_pBufferSkillFilter4Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter5", (CComponent**)&m_pBufferSkillFilter5Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter6", (CComponent**)&m_pBufferSkillFilter6Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter7", (CComponent**)&m_pBufferSkillFilter7Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter8", (CComponent**)&m_pBufferSkillFilter8Com)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter9", (CComponent**)&m_pBufferSkillFilter9Com)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_SkillBoard::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix		matTmp, matProj;
	D3DXMatrixIdentity(&matTmp);

	// UI는 직교투영을 해야하기 때문에 이렇게 처리해준다.
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixOrthoLH(&matProj, g_iWinCX, g_iWinCY, 0.f, 1.f);
	
	if (0 == iTargetTextureIdx)
	{
		m_pTextureSkillBoardCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillBoardCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (1 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter1Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[0]);
	}
	else if (2 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter2Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[1]);
	}
	else if (3 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter3Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[2]);
	}
	else if (4 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter4Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[3]);
	}
	else if (5 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter5Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[4]);
	}
	else if (6 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter6Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[5]);
	}
	else if (7 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter7Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[6]);
	}
	else if (8 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter8Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[7]);
	}
	else if (9 == iTargetTextureIdx)
	{
		m_pTextureSkillFilterCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillFilter9Com->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fCool", m_fCalculatedCoolTime[8]);
	}

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_SkillBoard::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pTextureSkillBoardCom)
		return E_FAIL;

	if (nullptr == m_pTransformSkillBoardCom)
		return E_FAIL;

	if (nullptr == m_pBufferSkillBoardCom)
		return E_FAIL;

	if (nullptr == m_pTextureSkillFilterCom)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter1Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter1Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter2Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter2Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter3Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter3Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter4Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter4Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter5Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter5Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter6Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter6Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter7Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter7Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter8Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter8Com)
		return E_FAIL;

	if (nullptr == m_pTransformSkillFilter9Com)
		return E_FAIL;

	if (nullptr == m_pBufferSkillFilter9Com)
		return E_FAIL;
	
	return NOERROR;
}

void CUI_SkillBoard::Init_Filters_Transform()
{
	// 스킬 판때기 텍스쳐.
	m_pTransformSkillBoardCom->Set_Scaling((g_iWinCX * 0.68f), (g_iWinCY * 0.15f), 0.f);
	m_pTransformSkillBoardCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.005f), -(g_iWinCY * 0.4f), 0.f));

	// 쿨타임용 필터 텍스쳐.
	m_pTransformSkillFilter1Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter1Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.1845f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter2Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter2Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.1525f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter3Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter3Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.1205f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter4Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter4Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.0885f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter5Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter5Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * 0.0565f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter6Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter6Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * -0.0455f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter7Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter7Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * -0.0775f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter8Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter8Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * -0.1095f), -(g_iWinCY * 0.429f), 0.f));

	m_pTransformSkillFilter9Com->Set_Scaling((g_iWinCX * 0.044f), (g_iWinCY * 0.075f), 0.f);
	m_pTransformSkillFilter9Com->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-(g_iWinCX * -0.0110f), -(g_iWinCY * 0.429f), 0.f));

}

void CUI_SkillBoard::Renewal_PlayerSkill_CoolTime()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

	{
		m_fMaxCoolTime[0] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::CutHead);;
		m_fCurrentCoolTime[0] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::CutHead);
		m_fCalculatedCoolTime[0] = m_fCurrentCoolTime[0] / m_fMaxCoolTime[0];
	}
	{
		m_fMaxCoolTime[1] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::CuttingSlash);;
		m_fCurrentCoolTime[1] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::CuttingSlash);
		m_fCalculatedCoolTime[1] = m_fCurrentCoolTime[1] / m_fMaxCoolTime[1];
	}
	{
		m_fMaxCoolTime[2] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::FlatBlade);;
		m_fCurrentCoolTime[2] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::FlatBlade);
		m_fCalculatedCoolTime[2] = m_fCurrentCoolTime[2] / m_fMaxCoolTime[2];
	}
	{
		m_fMaxCoolTime[3] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::HandySlash);;
		m_fCurrentCoolTime[3] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::HandySlash);
		m_fCalculatedCoolTime[3] = m_fCurrentCoolTime[3] / m_fMaxCoolTime[3];
	}
	{
		m_fMaxCoolTime[4] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::JawBreaker);;
		m_fCurrentCoolTime[4] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::JawBreaker);
		m_fCalculatedCoolTime[4] = m_fCurrentCoolTime[4] / m_fMaxCoolTime[4];
	}
	{
		m_fMaxCoolTime[5] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::StingerBlade);;
		m_fCurrentCoolTime[5] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::StingerBlade);
		m_fCalculatedCoolTime[5] = m_fCurrentCoolTime[5] / m_fMaxCoolTime[5];
	}
	{
		m_fMaxCoolTime[6] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::RagingStrike);;
		m_fCurrentCoolTime[6] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::RagingStrike);
		m_fCalculatedCoolTime[6] = m_fCurrentCoolTime[6] / m_fMaxCoolTime[6];
	}
	{
		m_fMaxCoolTime[7] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::DrawSword);;
		m_fCurrentCoolTime[7] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::DrawSword);
		m_fCalculatedCoolTime[7] = m_fCurrentCoolTime[7] / m_fMaxCoolTime[7];
	}
	{
		m_fMaxCoolTime[8] = pPlayer->Get_MaxCoolTimeInfo(CPlayer::PLAYER_ANI::Tumbling);;
		m_fCurrentCoolTime[8] = pPlayer->Get_CurrentCoolTimeInfo(CPlayer::PLAYER_ANI::Tumbling);
		m_fCalculatedCoolTime[8] = m_fCurrentCoolTime[8] / m_fMaxCoolTime[8];
	}
}

CUI_SkillBoard * CUI_SkillBoard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_SkillBoard* pInstance = new CUI_SkillBoard(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_SkillBoard Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_SkillBoard::Clone(void * pArg)
{
	CUI_SkillBoard* pInstance = new CUI_SkillBoard(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_SkillBoard Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillBoard::Free()
{
	Safe_Release(m_pTransformSkillBoardCom);
	Safe_Release(m_pBufferSkillBoardCom);
	Safe_Release(m_pTextureSkillBoardCom);

	Safe_Release(m_pTextureSkillFilterCom);

	Safe_Release(m_pTransformSkillFilter1Com);
	Safe_Release(m_pBufferSkillFilter1Com);

	Safe_Release(m_pTransformSkillFilter2Com);
	Safe_Release(m_pBufferSkillFilter2Com);

	Safe_Release(m_pTransformSkillFilter3Com);
	Safe_Release(m_pBufferSkillFilter3Com);

	Safe_Release(m_pTransformSkillFilter4Com);
	Safe_Release(m_pBufferSkillFilter4Com);

	Safe_Release(m_pTransformSkillFilter5Com);
	Safe_Release(m_pBufferSkillFilter5Com);

	Safe_Release(m_pTransformSkillFilter6Com);
	Safe_Release(m_pBufferSkillFilter6Com);

	Safe_Release(m_pTransformSkillFilter7Com);
	Safe_Release(m_pBufferSkillFilter7Com);

	Safe_Release(m_pTransformSkillFilter8Com);
	Safe_Release(m_pBufferSkillFilter8Com);

	Safe_Release(m_pTransformSkillFilter9Com);
	Safe_Release(m_pBufferSkillFilter9Com);

	CUI::Free();
}
