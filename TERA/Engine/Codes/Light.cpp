#include "..\Headers\Light.h"
#include "Buffer_ScreenTex.h"

CLight::CLight(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CLight::Ready_Light(const D3DLIGHT9 * pLightInfo)
{
	m_LightInfo = *pLightInfo;

	D3DVIEWPORT9	ViewPort;

	m_pGraphic_Device->GetViewport(&ViewPort);

	m_pBuffer = CBuffer_ScreenTex::Create(m_pGraphic_Device, 0.0f - 0.5f, 0.0f - 0.5f, ViewPort.Width, ViewPort.Height);
	if (nullptr == m_pBuffer)
		return E_FAIL;

	return NOERROR;
}

HRESULT CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	_uint	iPass = 0;

	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
	{
		iPass = 0;
		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
	}
	else if (D3DLIGHT_POINT == m_LightInfo.Type)
	{
		iPass = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
	}
	
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_LightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_LightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&m_LightInfo.Specular);
	
	_matrix		matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->CommitChanges();

	pEffect->BeginPass(iPass);

	m_pBuffer->Render_Buffer();

	pEffect->EndPass();

	return NOERROR;
}

CLight * CLight::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo)
{
	CLight* pInstance = new CLight(pGraphic_Device);

	if (FAILED(pInstance->Ready_Light(pLightInfo)))
	{
		_MSGBOX("CLight Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pGraphic_Device);
}
