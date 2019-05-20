#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"
#include "EventManagerTool.h"
#include "Buffer_Terrain_Tool.h"
#include "Texture_Tool.h"

_BEGIN(Engine)
class CTransform;
 // 그리기를 위한 리소스로 정점 버퍼를 추가한다.
class CRenderer; //백로고를 렌더그룹에 추가하기 위해.and Render함수를 호출할 수 있도록 
_END

class CTerrain final : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:	// Getter
	CTransform*		GetTransformCom() { return m_pTransformCom; }

public:
	HRESULT	Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail);
	HRESULT	Reset_Texture(_tchar* pFilePath);
private:
	CTransform*				m_pTransformCom = nullptr;
	CBuffer_Terrain_Tool*	m_pBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTexture_Tool*			m_pTextureCom = nullptr;

	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
private:
	HRESULT Add_Component();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
//private:
//	// VIBuffer와 Buffer_Terrain을 가져와서 Terrain 오브젝트에 이식한다.
//	// Cause 값을 바꿀때마다 갱신을 해주는데 일일이 찾기 번거롭기 때문.
//	HRESULT	Ready_Terrain_Buffer();
//	HRESULT	Ready_Terrain_Buffer(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail);
//	HRESULT Ready_VIBuffer();
//	void	Render_Buffer();
//
//	void	ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut);
	void	DrawAxis();
//private:
//	_uint							m_iNumVerticesX = 0;
//	_uint							m_iNumVerticesZ = 0;
//	_float							m_fInterval = 0.f;
//	_float							m_fDetail = 1.f;
//
//	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
//	_uint							m_iVtxSize = 0;
//	_uint							m_iNumVertices = 0;
//	_ulong							m_dwVtxFVF = 0;
//	_uint							m_iNumPolygons = 0;
//
//	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
//	_uint							m_iIndexSize = 0;
//	D3DFORMAT						m_Format = D3DFORMAT(0);
//	_vec3*							m_pPositions = nullptr; // 정점들의 최초 위치만 보관하는 배열.
//	void*							m_pIndices = nullptr;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();		
};

