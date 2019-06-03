#pragma once
#include "Base.h"

// 네비게이션메시를 구성하는 삼각형 하나.

_BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum EDGE { EDGE_AB, EDGE_BC, EDGE_CA, EDGE_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCell() = default;
public: // Getter
	const _vec3*	Get_Point(POINT ePoint) const { return &m_vPoint[ePoint]; }
	const CCell*	Get_Neighbor(NEIGHBOR eNeighbor) const { return m_pNeighbor[eNeighbor]; }
	_uint			Get_Index() const { return m_iIndex; }
public:	// Setter
	void			Set_Neighbor(NEIGHBOR eNeighbor, CCell* pCell) { m_pNeighbor[eNeighbor] = pCell; Safe_AddRef(m_pNeighbor[eNeighbor]); }
public:
	HRESULT			Ready_Cell(const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint iIndex);
	void			Render_Cell();
public:
	_bool			isIn(const _vec3* pPosition, EDGE* pOutEdge);
	_bool			isNeighbor(const _vec3* pSourPoint, const _vec3* pDestPoint, CCell* pCell);

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	LPD3DXLINE			m_pLine = nullptr;
	_int				m_iIndex = 0;
private:
	_vec3		m_vPoint[POINT_END];
	_vec3		m_vEdge[EDGE_END];
	CCell*		m_pNeighbor[NEIGHBOR_END];

public:
	static CCell*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, _uint iIndex);
	virtual void	Free();
};

_END