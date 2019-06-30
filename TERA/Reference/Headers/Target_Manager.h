#pragma once
#include "Base.h"

_BEGIN(Engine)

class CTarget;
class _DLL_EXPORTS CTarget_Manager final : public CBase
{
	_DECLARE_SINGLETON(CTarget_Manager)
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;
public:
	HRESULT Add_RenderTarget(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Clear_Color);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);

#ifdef _DEBUG 
	HRESULT Ready_Debug_Buffer(const _tchar* pTargetTag, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
#endif

private:
	map<const _tchar*, CTarget*>					m_mapTarget;
	typedef map<const _tchar*, CTarget*>			MAPTARGET;
private:
	map<const _tchar*, list<CTarget*>>				m_mapMRT;
	typedef map < const _tchar*, list<CTarget*>>	MAPMRT;
public:
	CTarget*		Find_Target(const _tchar* pTargetTag);
	list<CTarget*>*	Find_MRT(const _tchar* pMRTTag);
public:
	virtual void	Free();
};

_END