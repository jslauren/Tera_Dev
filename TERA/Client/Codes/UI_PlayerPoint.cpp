#include "stdafx.h"
#include "..\Headers\UI_PlayerPoint.h"

_USING(Client)

CUI_PlayerPoint::CUI_PlayerPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
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
	m_pTransformPointBoardCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, -(g_iWinCY * 0.305f), 0.f));

	return NOERROR;
}

_int CUI_PlayerPoint::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

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

	//{
	//	if (FAILED(SetUp_ConstantTable(pEffect, 1)))
	//		return E_FAIL;

	//	pEffect->Begin(nullptr, 0);
	//	pEffect->BeginPass(0);

	//	m_pBufferHpFilterCom->Render_Buffer();

	//	pEffect->EndPass();
	//	pEffect->End();
	//}

	//{
	//	if (FAILED(SetUp_ConstantTable(pEffect, 2)))
	//		return E_FAIL;

	//	pEffect->Begin(nullptr, 0);
	//	pEffect->BeginPass(0);

	//	m_pBufferMpFilterCom->Render_Buffer();

	//	pEffect->EndPass();
	//	pEffect->End();
	//}

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_PlayerPoint::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformPointBoard", (CComponent**)&m_pTransformPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformHpFilter", (CComponent**)&m_pTransformHpFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformMpFilter", (CComponent**)&m_pTransformMpFilterCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Point_Board", L"Com_BufferPoint_Board", (CComponent**)&m_pBufferPointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_HpFilter", L"Com_BufferHpFilter", (CComponent**)&m_pBufferHpFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_MpFilter", L"Com_BufferMpFilter", (CComponent**)&m_pBufferMpFilterCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Point_Board", L"Com_TexturePoint_Board", (CComponent**)&m_pTexturePointBoardCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_HpFilter", L"Com_TextureHpFilter", (CComponent**)&m_pTextureHpFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_MpFilter", L"Com_TextureMpFilter", (CComponent**)&m_pTextureMpFilterCom)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CUI_PlayerPoint::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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
		m_pTexturePointBoardCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformPointBoardCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
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

	if (nullptr == m_pTransformHpFilterCom)
		return E_FAIL;

	if (nullptr == m_pBufferHpFilterCom)
		return E_FAIL;

	if (nullptr == m_pTextureHpFilterCom)
		return E_FAIL;

	if (nullptr == m_pTransformMpFilterCom)
		return E_FAIL;

	if (nullptr == m_pBufferMpFilterCom)
		return E_FAIL;

	if (nullptr == m_pTextureMpFilterCom)
		return E_FAIL;


	return NOERROR;
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

	Safe_Release(m_pTransformHpFilterCom);
	Safe_Release(m_pBufferHpFilterCom);
	Safe_Release(m_pTextureHpFilterCom);

	Safe_Release(m_pTransformMpFilterCom);
	Safe_Release(m_pBufferMpFilterCom);
	Safe_Release(m_pTextureMpFilterCom);

	CUI::Free();
}
