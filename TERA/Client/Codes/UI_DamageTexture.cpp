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

	//m_pTransformThsnCom->Set_Scaling(m_fFontSize, m_fFontSize, 0.f);
	//m_pTransformHndrCom->Set_Scaling(m_fFontSize, m_fFontSize, 0.f);
	//m_pTransformTenCom->Set_Scaling(m_fFontSize, m_fFontSize, 0.f);
	//m_pTransformOneCom->Set_Scaling(m_fFontSize, m_fFontSize, 0.f);

	return NOERROR;
}

_int CUI_DamageTexture::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeAcc += (fTimeDelta * 1.25f);
	
	if (m_bIsSub == false)
	{
		if (m_fCurrentFontSize <= m_fMaxFontSize)
			m_fCurrentFontSize += m_fTimeAcc;

		else if (m_fCurrentFontSize >= m_fMaxFontSize)
			m_bIsSub = true;
	}
	else if (m_bIsSub == true)
	{
		if (m_fCurrentFontSize > m_fMinFontSize)
			m_fCurrentFontSize -= m_fTimeAcc;

		else if (m_fCurrentFontSize <= m_fMinFontSize)
		{
			m_fFontStayTimeAcc += fTimeDelta;

			if (m_fFontStayTimeAcc >= 0.075f)
			{
				_vec3 Temp;

				m_fFontPosition += (fTimeDelta);

				if (m_fMaxFontPosition > m_fFontPosition)
				{
					m_fFontAlpha = m_fFontPosition;

					Temp = *(m_pTransformThsnCom->Get_StateInfo(CTransform::STATE_POSITION));
					Temp.y += m_fFontPosition;
					m_pTransformThsnCom->Set_StateInfo(CTransform::STATE_POSITION, &Temp);

					Temp = *(m_pTransformHndrCom->Get_StateInfo(CTransform::STATE_POSITION));
					Temp.y += m_fFontPosition;
					m_pTransformHndrCom->Set_StateInfo(CTransform::STATE_POSITION, &Temp);

					Temp = *(m_pTransformTenCom->Get_StateInfo(CTransform::STATE_POSITION));
					Temp.y += m_fFontPosition;
					m_pTransformTenCom->Set_StateInfo(CTransform::STATE_POSITION, &Temp);

					Temp = *(m_pTransformOneCom->Get_StateInfo(CTransform::STATE_POSITION));
					Temp.y += m_fFontPosition;
					m_pTransformOneCom->Set_StateInfo(CTransform::STATE_POSITION, &Temp);
				}
				else if (m_fMaxFontPosition <= m_fFontPosition)
					return 1;
			}			
		}
	}

	m_pTransformThsnCom->Set_Scaling(m_fCurrentFontSize, m_fCurrentFontSize, 0.f);
	m_pTransformHndrCom->Set_Scaling(m_fCurrentFontSize, m_fCurrentFontSize, 0.f);
	m_pTransformTenCom->Set_Scaling(m_fCurrentFontSize, m_fCurrentFontSize, 0.f);
	m_pTransformOneCom->Set_Scaling(m_fCurrentFontSize, m_fCurrentFontSize, 0.f);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int(0);
}

_int CUI_DamageTexture::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
	
	return _int();
}

HRESULT CUI_DamageTexture::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	for (size_t i = 0; i < m_iNumberUnit; ++i)
	{
		if (FAILED(SetUp_ConstantTable(pEffect, i)))
			return E_FAIL;

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(6);

		if (i == 0)
			m_pBufferOneCom->Render_Buffer();

		else if(i == 1)
			m_pBufferTenCom->Render_Buffer();

		else if (i == 2)
			m_pBufferHndrCom->Render_Buffer();

		else if (i == 3)
			m_pBufferThsnCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

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
	D3DXMatrixOrthoLH(&matProj, (_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f);

	if (0 == iTargetTextureIdx)
	{
		// 일의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[0]);
		
		pEffect->SetMatrix("g_matWorld", m_pTransformOneCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
		pEffect->SetFloat("g_fDamageFontAlpha", (1.f - m_fFontAlpha));
	}
	else if (1 == iTargetTextureIdx)
	{
		// 십의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[1]);

		pEffect->SetMatrix("g_matWorld", m_pTransformTenCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (2 == iTargetTextureIdx)
	{
		// 백의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[2]);

		pEffect->SetMatrix("g_matWorld", m_pTransformHndrCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	else if (3 == iTargetTextureIdx)
	{
		// 천의 자리.
		m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[3]);

		pEffect->SetMatrix("g_matWorld", m_pTransformThsnCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}

	// Old
	{
		//if (1 == iTargetTextureIdx)
		//{
		//	// 백의 자리.
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[2]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformHndrCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);

		//	// 십의 자리.
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[1]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformTenCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);

		//	// 일의 자리
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[0]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformOneCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);
		//}
		//else if (2 == iTargetTextureIdx)
		//{
		//	// 천의 자리.
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[3]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformThsnCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);

		//	// 백의 자리.
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[2]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformHndrCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);

		//	// 십의 자리.
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[1]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformTenCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);

		//	// 일의 자리
		//	m_pTextureDamageCom->SetUp_OnShader(pEffect, "g_BaseTexture", m_iSeprateDamage[0]);

		//	pEffect->SetMatrix("g_matWorld", m_pTransformOneCom->Get_WorldMatrixPointer());
		//	pEffect->SetMatrix("g_matView", &matTmp);
		//	pEffect->SetMatrix("g_matProj", &matProj);
		//}
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

	if (nullptr == m_pTransformTenCom)
		return E_FAIL;

	if (nullptr == m_pBufferTenCom)
		return E_FAIL;

	if (nullptr == m_pTransformHndrCom)
		return E_FAIL;

	if (nullptr == m_pBufferHndrCom)
		return E_FAIL;

	if (nullptr == m_pTransformThsnCom)
		return E_FAIL;

	if (nullptr == m_pBufferThsnCom)
		return E_FAIL;
	
	return NOERROR;
}

void CUI_DamageTexture::SeprateDamageValue()
{
	wsprintf(m_szDamageValue, L"%d", m_iDamageValue);

	if (lstrlen(m_szDamageValue) == 4)
	{
		m_iNumberUnit = 4;

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
		m_iNumberUnit = 3;

		m_iSeprateDamage[2] = m_iDamageValue / 100;
		m_iDamageValue %= 100;

		m_iSeprateDamage[1] = m_iDamageValue / 10;
		m_iDamageValue %= 10;

		m_iSeprateDamage[0] = m_iDamageValue;
	}
}

void CUI_DamageTexture::SetDamageTransform()
{
	_vec3 vTempPos;

	if (m_iNumberUnit == 3)
	{
		vTempPos = m_vPosition;
		vTempPos.x -= 17.5f;
		m_pTransformHndrCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);

		vTempPos = m_vPosition;
		m_pTransformTenCom->Set_StateInfo(CTransform::STATE_POSITION, &m_vPosition);

		vTempPos = m_vPosition;
		vTempPos.x += 17.5f;
		m_pTransformOneCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);
	}
	else if (m_iNumberUnit == 4)
	{
		vTempPos = m_vPosition;
		vTempPos.x -= 26.25f;
		m_pTransformThsnCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);

		vTempPos = m_vPosition;
		vTempPos.x -= 8.75f;
		m_pTransformHndrCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);

		vTempPos = m_vPosition;
		vTempPos.x += 8.75f;
		m_pTransformTenCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);

		vTempPos = m_vPosition;
		vTempPos.x += 26.25f;
		m_pTransformOneCom->Set_StateInfo(CTransform::STATE_POSITION, &vTempPos);
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

	Safe_Release(m_pTransformTenCom);
	Safe_Release(m_pBufferTenCom);

	Safe_Release(m_pTransformHndrCom);
	Safe_Release(m_pBufferHndrCom);

	Safe_Release(m_pTransformThsnCom);
	Safe_Release(m_pBufferThsnCom);

	CUI::Free();
}

