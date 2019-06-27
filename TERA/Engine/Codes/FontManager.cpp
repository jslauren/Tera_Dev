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
	lstrcpy(tFontInfo.FaceName, L"¸¼Àº°íµñÃ¼");
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

}

void CFontManager::RenderFont(FONT_ID eFontID, _vec3 vPosition, _tchar * pFontContents)
{
	//wsprintf(m_szStageCnt, L"%d/%d", m_iCurrentStage, m_iTotalStage);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	_matrix	matTrans;

	D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, 0.f);

	m_pSprite->SetTransform(&matTrans);

	m_pFont[eFontID]->DrawTextW(m_pSprite, pFontContents, lstrlen(pFontContents), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pSprite->End();
}

void CFontManager::Free()
{
	Safe_Release(m_pGraphic_Device);

	for (size_t i = 0; i < FONT_END; ++i)
		Safe_Release(m_pFont[i]);

	if (0 != Safe_Release(m_pSprite))
		_MSGBOX("m_pSprite Release Failed");
}
