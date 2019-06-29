#include "stdafx.h"
#include "..\Headers\UI_SkillBoard.h"
#include "Management.h"
#include "Camera_Dynamic.h"

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

	m_pTransformSkillBoardCom->Set_Scaling((g_iWinCX * 0.68f), (g_iWinCY * 0.15f), 0.f);
	m_pTransformSkillBoardCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), -(g_iWinCY * 0.4f), 0.f));

	m_pTransformSkillFilterCom->Set_Scaling((g_iWinCX * 0.01f), (g_iWinCY * 0.15f), 0.f);
	m_pTransformSkillFilterCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), -(g_iWinCY * 0.4f), 0.f));

	return NOERROR;
}

_int CUI_SkillBoard::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

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


		if (FAILED(SetUp_ConstantTable(pEffect)))
			return E_FAIL;

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		m_pBufferSkillBoardCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);

		//{
		//	if (FAILED(SetUp_ConstantTable(pEffect, 1)))
		//		return E_FAIL;

		//	pEffect->Begin(nullptr, 0);
		//	pEffect->BeginPass(2);

		//	m_pBufferSkillFilterCom->Render_Buffer();

		//	pEffect->EndPass();
		//	pEffect->End();
		//}
	}

	return NOERROR;
}

HRESULT CUI_SkillBoard::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillBoard", (CComponent**)&m_pTransformSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformSkillFilter", (CComponent**)&m_pTransformSkillFilterCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillBoard", L"Com_BufferSkill_Board", (CComponent**)&m_pBufferSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_SkillFilter", L"Com_BufferSkill_Filter", (CComponent**)&m_pBufferSkillFilterCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_SkillBoard", L"Com_TextureSkill_Board", (CComponent**)&m_pTextureSkillBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_SkillFilter", L"Com_TextureSkill_Filter", (CComponent**)&m_pTextureSkillFilterCom)))
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

	if (1 == iTargetTextureIdx)
	{
		m_pTextureSkillBoardCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillBoardCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (2 == iTargetTextureIdx)
	{
		m_pTextureSkillBoardCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformSkillBoardCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetFloat("g_fCoolTimeDegree", _float(45.f));
	}

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_SkillBoard::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	return NOERROR;
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

	Safe_Release(m_pTransformSkillFilterCom);
	Safe_Release(m_pBufferSkillFilterCom);
	Safe_Release(m_pTextureSkillFilterCom);

	CUI::Free();
}
