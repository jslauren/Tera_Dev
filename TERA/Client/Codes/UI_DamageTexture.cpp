#include "stdafx.h"
#include "..\Headers\UI_DamageTexture.h"

_USING(Client)

CUI_DamageTexture::CUI_DamageTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	ZeroMemory(m_szDamageValue, sizeof(_tchar) * 64);
}

CUI_DamageTexture::CUI_DamageTexture(const CUI_DamageTexture & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_DamageTexture::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_DamageTexture::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = (*(_vec4*)(pArg));

	// 받아온 위치 정보를 멤버변수를 통해 저장해 놓는다.
	m_vPosition.x = vInfo.x;
	m_vPosition.y = vInfo.y;
	m_vPosition.z = vInfo.z;

	m_iDamageValue = (_uint)vInfo.w;

	SeprateDamageValue();

	SetDamageTransform();
	
	return NOERROR;
}

_int CUI_DamageTexture::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CUI_DamageTexture::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_DamageTexture::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect, m_iNumberUnit)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 받은 데미지가 천 단위라면, 천 단위 버퍼도 렌더 한다.
	if (m_iNumberUnit == 2)
		m_pBufferThsnCom->Render_Buffer();

	// 그렇지 않다면, 백 단위 까지 렌더한다.
	m_pBufferHndrCom->Render_Buffer();
	m_pBufferTenCom->Render_Buffer();
	m_pBufferOneCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_DamageTexture::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform_Damage_One", (CComponent**)&m_pTransformOneCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform_Damage_Ten", (CComponent**)&m_pTransformTenCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform_Damage_Hndr", (CComponent**)&m_pTransformHndrCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform_Damage_Thsn", (CComponent**)&m_pTransformThsnCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Damage_One", L"Com_Buffer_Damage_One", (CComponent**)&m_pBufferOneCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Damage_Ten", L"Com_Buffer_Damage_Ten", (CComponent**)&m_pBufferTenCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Damage_Hndr", L"Com_Buffer_Damage_Hndr", (CComponent**)&m_pBufferHndrCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Damage_Thsn", L"Com_Buffer_Damage_Thsn", (CComponent**)&m_pBufferThsnCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage", L"Com_Texture_Damage", (CComponent**)&m_pTextureDamageCom)))
		return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage_One", L"Com_Texture_Damage_One", (CComponent**)&m_pTextureOneCom)))
	//	return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage_Ten", L"Com_Texture_Damage_Ten", (CComponent**)&m_pTextureTenCom)))
	//	return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage_Hndr", L"Com_Texture_Damage_Hndr", (CComponent**)&m_pTextureHndrCom)))
	//	return E_FAIL;

	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage_Thsn", L"Com_Texture_Damage_Thsn", (CComponent**)&m_pTextureThsnCom)))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CUI_DamageTexture::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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
		// 백의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[2]);

		pEffect->SetMatrix("g_matWorld", m_pTransformHndrCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		// 십의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[1]);

		pEffect->SetMatrix("g_matWorld", m_pTransformTenCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		// 일의 자리
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[0]);

		pEffect->SetMatrix("g_matWorld", m_pTransformOneCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (2 == iTargetTextureIdx)
	{
		// 천의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[3]);

		pEffect->SetMatrix("g_matWorld", m_pTransformThsnCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		// 백의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[2]);

		pEffect->SetMatrix("g_matWorld", m_pTransformHndrCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		// 십의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[1]);

		pEffect->SetMatrix("g_matWorld", m_pTransformTenCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);

		// 일의 자리
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[0]);

		pEffect->SetMatrix("g_matWorld", m_pTransformOneCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_DamageTexture::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pTextureDamageCom)
		return E_FAIL;

	if (nullptr == m_pTransformOneCom)
		return E_FAIL;

	if (nullptr == m_pBufferOneCom)
		return E_FAIL;

	//if (nullptr == m_pTextureOneCom)
	//	return E_FAIL;

	if (nullptr == m_pTransformTenCom)
		return E_FAIL;

	if (nullptr == m_pBufferTenCom)
		return E_FAIL;

	//if (nullptr == m_pTextureTenCom)
	//	return E_FAIL;

	if (nullptr == m_pTransformHndrCom)
		return E_FAIL;

	if (nullptr == m_pBufferHndrCom)
		return E_FAIL;

	//if (nullptr == m_pTextureHndrCom)
	//	return E_FAIL;

	if (nullptr == m_pTransformThsnCom)
		return E_FAIL;

	if (nullptr == m_pBufferThsnCom)
		return E_FAIL;

	//if (nullptr == m_pTextureThsnCom)
	//	return E_FAIL;
	
	return NOERROR;
}

void CUI_DamageTexture::SeprateDamageValue()
{
	wsprintf(m_szDamageValue, L"%d", m_iDamageValue);

	if (lstrlen(m_szDamageValue) == 4)
	{
		m_iNumberUnit = 2;

		m_iSeprateDamage[3] = m_iDamageValue / 1000;
		m_iDamageValue %= 1000;

		m_iSeprateDamage[2] = m_iDamageValue / 100;
		m_iDamageValue %= 100;

		m_iSeprateDamage[1] = m_iDamageValue / 10;
		m_iDamageValue %= 10;

		m_iSeprateDamage[0] = m_iDamageValue;
	}
	else if (lstrlen(m_szDamageValue) == 3)
	{
		m_iNumberUnit = 1;

		m_iSeprateDamage[2] = m_iDamageValue / 100;
		m_iDamageValue %= 100;

		m_iSeprateDamage[1] = m_iDamageValue / 10;
		m_iDamageValue %= 10;

		m_iSeprateDamage[0] = m_iDamageValue;
	}
}

void CUI_DamageTexture::SetDamageTransform()
{
	if (m_iNumberUnit == 1)
	{
		m_pTransformHndrCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
		m_pTransformTenCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
		m_pTransformOneCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
	}
	else if (m_iNumberUnit == 2)
	{
		m_pTransformThsnCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
		m_pTransformHndrCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
		m_pTransformTenCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
		m_pTransformOneCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.f), (g_iWinCY * 0.f), 0.f));
	}
}

CUI_DamageTexture * CUI_DamageTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	CUI_DamageTexture* pInstance = new CUI_DamageTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_DamageTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_DamageTexture::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CUI_DamageTexture::Free()
{
	Safe_Release(m_pTextureDamageCom);

	Safe_Release(m_pTransformOneCom);
	Safe_Release(m_pBufferOneCom);
//	Safe_Release(m_pTextureOneCom);

	Safe_Release(m_pTransformTenCom);
	Safe_Release(m_pBufferTenCom);
//	Safe_Release(m_pTextureTenCom);

	Safe_Release(m_pTransformHndrCom);
	Safe_Release(m_pBufferHndrCom);
//	Safe_Release(m_pTextureHndrCom);

	Safe_Release(m_pTransformThsnCom);
	Safe_Release(m_pBufferThsnCom);
//	Safe_Release(m_pTextureThsnCom);

	CUI::Free();
}

