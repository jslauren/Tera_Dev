#pragma once
#include "Component.h"

_BEGIN(Engine)

class CShader;
class CGameObject;
class CLight_Manager;
class CTarget_Manager;
class CBuffer_ScreenTex;
class _DLL_EXPORTS CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CRenderer() = default;
public:
	HRESULT Ready_Renderer();
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT Render_RenderGroup();
private:
	list<CGameObject*>			m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;
private:
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CBuffer_ScreenTex*			m_pBuffer = nullptr;
	CShader*					m_pShader_LightAcc = nullptr;
	CShader*					m_pShader_Blend = nullptr;
private:
	HRESULT Render_Priority();
	HRESULT Render_NoneAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
private:
	HRESULT Render_Deferrerd();
	HRESULT Render_LightAcc();
	HRESULT Render_Blend();

private:
	_bool	m_bIsRendering = false;

public:
	static CRenderer*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr);
	virtual void		Free();

};

_END