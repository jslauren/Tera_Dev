#include "stdafx.h"
#include "..\Headers\Texture_Tool.h"

CTexture_Tool::CTexture_Tool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CTexture_Tool::Ready_Texture(TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture)
{
	m_vecTexture.reserve(iNumTexture);

	for (size_t i = 0; i < iNumTexture; ++i)
	{
		_tchar		szFileName[MAX_PATH] = L"";

		IDirect3DBaseTexture9*		pTexture = nullptr;

		wsprintf(szFileName, pFileName, i);

		if (TYPE_GENERAL == eType)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
		}
		else if (TYPE_CUBE == eType)
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
		}

		m_vecTexture.push_back(pTexture);
	}

	return NOERROR;
}

HRESULT CTexture_Tool::SetUp_OnGraphicDev(const _uint & iIndex)
{
	if (m_vecTexture.size() <= iIndex ||
		nullptr == m_vecTexture[iIndex] ||
		nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetTexture(0, m_vecTexture[iIndex])))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTexture_Tool::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName, const _uint & iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_vecTexture[iIndex]);

	return NOERROR;
}

HRESULT CTexture_Tool::Reset_Texture(TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture)
{
	for (auto& pTexture : m_vecTexture)
	{
		Safe_Release(pTexture);
	}
	m_vecTexture.clear();

	m_vecTexture.reserve(iNumTexture);

	for (size_t i = 0; i < iNumTexture; ++i)
	{
		_tchar		szFileName[MAX_PATH] = L"";

		IDirect3DBaseTexture9*		pTexture = nullptr;

		wsprintf(szFileName, pFileName, i);

		if (TYPE_GENERAL == eType)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
		}
		else if (TYPE_CUBE == eType)
		{
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
		}

		m_vecTexture.push_back(pTexture);
	}

	return NOERROR;
}

CTexture_Tool * CTexture_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture)
{
	CTexture_Tool* pInstance = new CTexture_Tool(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(eType, pFileName, iNumTexture)))
	{
		_MSGBOX("CTexture_Tool Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture_Tool::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CTexture_Tool::Free()
{
	for (auto& pTexture : m_vecTexture)
	{
		Safe_Release(pTexture);
	}
	m_vecTexture.clear();

	CComponent::Free();
}
