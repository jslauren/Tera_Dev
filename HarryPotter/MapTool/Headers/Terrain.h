#pragma once
#include "Tool_Defines.h"
#include "GameObject.h"
#include "EventManagerTool.h"
#include "Buffer_Terrain_Tool.h"
#include "Texture_Tool.h"

_BEGIN(Engine)
class CTransform;
 // �׸��⸦ ���� ���ҽ��� ���� ���۸� �߰��Ѵ�.
class CRenderer; //��ΰ� �����׷쿡 �߰��ϱ� ����.and Render�Լ��� ȣ���� �� �ֵ��� 
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
//	// VIBuffer�� Buffer_Terrain�� �����ͼ� Terrain ������Ʈ�� �̽��Ѵ�.
//	// Cause ���� �ٲܶ����� ������ ���ִµ� ������ ã�� ���ŷӱ� ����.
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
//	_vec3*							m_pPositions = nullptr; // �������� ���� ��ġ�� �����ϴ� �迭.
//	void*							m_pIndices = nullptr;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();		
};

