#pragma once
#include "Component.h"

// 내 길을 표현하는 삼각형(셀)들을 여러개 가지고있는 객체.

_BEGIN(Engine)

class CCell;
class _DLL_EXPORTS CNavigation final : public CComponent
{
private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	HRESULT SetUp_CurrentIndex(_uint iIndex) { m_iCurrentIndex = iIndex; return NOERROR; }
	void	SetUp_CurrentIndexLoop(_vec3 vPos);
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath);
	virtual HRESULT Ready_Component(void* pArg);
	virtual void	Render_Navigation();
public:
	_bool	Move_OnNavigation(const _vec3* pPosition, const _vec3* pDir, _float fTimeDelta, _uint* pOutIndex);
	HRESULT SetUp_Neighbor();
	_float	Compute_HeightOnNavi(_vec3* _pPlayerPos);

private:
	vector<CCell*>			m_vecCell;
	typedef vector<CCell*>	VECCELL;
private:
	_uint					m_iCurrentIndex = 0;
public:
	static CNavigation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

_END