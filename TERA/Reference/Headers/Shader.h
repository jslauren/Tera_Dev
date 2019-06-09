#pragma once
#include "Component.h"

_BEGIN(Engine)

class _DLL_EXPORTS CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	LPD3DXEFFECT	Get_EffectHandle() { return m_pEffect; }
public:
	HRESULT			Ready_Shader(const _tchar* pShaderFilePath);
private:
	// ���� ������ ���̴������� �����Ҽ� �ִ� ��ü.
	LPD3DXEFFECT	m_pEffect = nullptr;
	LPD3DXBUFFER	m_pBuffer;
public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

_END