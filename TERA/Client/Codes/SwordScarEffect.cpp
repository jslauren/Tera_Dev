#include "stdafx.h"
#include "..\Headers\SwordScarEffect.h"
#include "Target_Manager.h"

_USING(Client)

CSwordScarEffect::CSwordScarEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect(pGraphic_Device)
{
}

CSwordScarEffect::CSwordScarEffect(const CSwordScarEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CSwordScarEffect::Ready_GameObject_Prototype()
{
	CEffect::Ready_GameObject_Prototype();

	return NOERROR;
}

HRESULT CSwordScarEffect::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_vColliderPos = (*(_vec4*)(pArg));

	m_pTransformCom->Set_Scaling(10.f, 10.f, 1.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_vColliderPos.x, m_vColliderPos.y, m_vColliderPos.z));

	return NOERROR;
}

_int CSwordScarEffect::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	SetUp_BillBoard();

	if (m_vColliderPos.w == 0)
		m_pTransformCom->Set_Angle_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK), D3DXToRadian(-30.f));
	else if (m_vColliderPos.w == 1)
		m_pTransformCom->Set_Angle_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK), D3DXToRadian(-90.f));

	m_fAlphaValue += (fTimeDelta * 1.5f);
	m_fScalingY += (fTimeDelta * 230.f);

	if (m_fScalingY > 45.f)
		return -1;

	return _int();
}

_int CSwordScarEffect::LateUpdate_GameObject(const _float & fTimeDelta)
{
	CEffect::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CSwordScarEffect::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTransformCom->Set_Scaling(3.f, m_fScalingY, 1.f);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(3);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CSwordScarEffect::Add_Component()
{
	CEffect::Add_Component();

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_SwordScarEffect", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CSwordScarEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));
	pEffect->SetFloat("g_fAlphaValue", (1.f - m_fAlphaValue));

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", _uint(m_fFrame));

	if (FAILED(CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return E_FAIL;

	Safe_Release(pEffect);

	return NOERROR;
}

CSwordScarEffect * CSwordScarEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSwordScarEffect* pInstance = new CSwordScarEffect(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CSwordScarEffect Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSwordScarEffect::Clone(void * pArg)
{
	CSwordScarEffect* pInstance = new CSwordScarEffect(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CSwordScarEffect Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordScarEffect::Free()
{
	CEffect::Free();
}
