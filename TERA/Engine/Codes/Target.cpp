#include "..\Headers\Target.h"
#include "Buffer_ScreenTex.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CTarget::Ready_Target(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Clear_Color)
{
	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, eFormat, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
		return E_FAIL;

	// 만들어낸 텍스쳐로부터 실제 사용할 렌더타겟을 얻어온다.
	if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
		return E_FAIL;

	m_Clear_Color = Clear_Color;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnGraphicDev(_uint iIndex)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOld_Surface);

	if (FAILED(m_pGraphic_Device->SetRenderTarget(iIndex, m_pTarget_Surface)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTarget_Texture);

	return NOERROR;
}

HRESULT CTarget::Release_OnGraphicDev(_uint iIndex)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOld_Surface);

	Safe_Release(m_pOld_Surface);

	if (GetKeyState(VK_RETURN) < 0)
	{
		if (0 == iIndex)
			D3DXSaveTextureToFile(L"../Bin/Diffuse.jpg", D3DXIFF_JPG, m_pTarget_Texture, nullptr);
		if (1 == iIndex)
			D3DXSaveTextureToFile(L"../Bin/Normal.jpg", D3DXIFF_JPG, m_pTarget_Texture, nullptr);
	}

	return NOERROR;
}

HRESULT CTarget::Clear_Target()
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pTarget_Surface)
		return E_FAIL;

	m_pGraphic_Device->GetRenderTarget(0, &m_pOld_Surface);

	m_pGraphic_Device->SetRenderTarget(0, m_pTarget_Surface);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_Clear_Color, 1.f, 0);

	m_pGraphic_Device->SetRenderTarget(0, m_pOld_Surface);

	Safe_Release(m_pOld_Surface);

	return NOERROR;
}

#ifdef _DEBUG
HRESULT CTarget::Ready_Debug_Buffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY)
{
	m_pDebugBuffer = CBuffer_ScreenTex::Create(m_pGraphic_Device, fStartX, fStartY, fSizeX, fSizeY);
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	return NOERROR;
}
HRESULT CTarget::Render_Debug_Buffer()
{
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, m_pTarget_Texture);
	m_pDebugBuffer->Render_Buffer();

	return NOERROR;
}
#endif

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Clear_Color)
{
	CTarget* pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iWidth, iHeight, eFormat, Clear_Color)))
	{
		_MSGBOX("CTarget Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTarget::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pDebugBuffer);

#endif // _DEBUG


	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTarget_Surface);
	Safe_Release(m_pTarget_Texture);
}
