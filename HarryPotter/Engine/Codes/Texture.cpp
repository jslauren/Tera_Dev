#include "..\Headers\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_vecTexture(rhs.m_vecTexture)
{
	for (auto& pTexture : m_vecTexture)
		pTexture->AddRef();
}


HRESULT CTexture::Ready_Texture(TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture)
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

HRESULT CTexture::SetUp_OnGraphicDev(const _uint & iIndex)
{
	if (m_vecTexture.size() <= iIndex ||
		nullptr == m_vecTexture[iIndex] ||
		nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->SetTexture(0, m_vecTexture[iIndex])))
		return E_FAIL;

	return E_NOTIMPL;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture)
{
	CTexture* pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_Texture(eType, pFileName, iNumTexture)))
	{
		_MSGBOX("CTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	for (auto& pTexture : m_vecTexture)
	{
		Safe_Release(pTexture);
	}
	m_vecTexture.clear();

	CComponent::Free();
}
