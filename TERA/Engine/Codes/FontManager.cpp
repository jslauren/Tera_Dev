#include "..\Headers\FontManager.h"

_IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
}

HRESULT CFontManager::ReadyFont(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(m_pGraphic_Device);

	if (FAILED(D3DXCreateSprite(pGraphic_Device, &m_pSprite)))
	{
		_MSGBOX("Sprite Create Failed");
		return E_FAIL;
	}

	// Font Setting Area //
	D3DXFONT_DESC	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Weight = FW_EXTRABOLD;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	tFontInfo.Height = 14;
	tFontInfo.Width = 9;
	lstrcpy(tFontInfo.FaceName, L"��������ü");
	if (FAILED(D3DXCreateFontIndirect(pGraphic_Device, &tFontInfo, &m_pFont[FONT_NAME])))
	{
		_MSGBOX("Font Create Failed")
			return E_FAIL;
	}

	tFontInfo.Height = 8;
	tFontInfo.Width = 3;
	if (FAILED(D3DXCreateFontIndirect(pGraphic_Device, &tFontInfo, &m_pFont[FONT_POINT_VALUE])))
	{
		_MSGBOX("Font Create Failed")
			return E_FAIL;
	}

	tFontInfo.Height = 13;
	tFontInfo.Width = 7;
	if (FAILED(D3DXCreateFontIndirect(pGraphic_Device, &tFontInfo, &m_pFont[FONT_SCRIPT])))
	{
		_MSGBOX("Font Create Failed")
			return E_FAIL;
	}

	return NOERROR;
}

void CFontManager::RenderFont(FONT_ID eFontID, _vec3 vPosition, _tchar * pFontContents, D3DCOLORVALUE dwColor)
{
	//wsprintf(m_szStageCnt, L"%d/%d", m_iCurrentStage, m_iTotalStage);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	_matrix	matTrans;

	D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, 0.f);

	m_pSprite->SetTransform(&matTrans);

	m_pFont[eFontID]->DrawTextW(m_pSprite, pFontContents, lstrlen(pFontContents), NULL, NULL, D3DCOLOR_COLORVALUE(dwColor.r, dwColor.g, dwColor.b, dwColor.a));

	m_pSprite->End();
}

void CFontManager::Free()
{
	for (size_t i = 0; i < FONT_END; ++i)
	{
		if (m_pFont[i] != nullptr)
		{
			if (0 != Safe_Release(m_pFont[i]))
				_MSGBOX("m_pSprite Release Failed");
		}
	}

	if (0 != Safe_Release(m_pSprite))
		_MSGBOX("m_pSprite Release Failed");

	Safe_Release(m_pGraphic_Device);

}
