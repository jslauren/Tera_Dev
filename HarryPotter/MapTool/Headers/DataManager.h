#pragma once
#include "Base.h"

class CDataManager final : public CBase
{
	_DECLARE_SINGLETON(CDataManager)

private:
	explicit CDataManager();
	virtual ~CDataManager() = default;

public:	// Setter
	void	Set_Picking_Variable(D3DFORMAT _Format, _uint _iNumPolygons, _vec3* _pPositions, void* _pIndices) {
		m_Format = _Format; m_iNumPolygons = _iNumPolygons; m_pPositions = _pPositions;  m_pIndices = _pIndices; }

public:
	// Terrain_Vertex_Variable
	D3DFORMAT	m_Format = D3DFORMAT(0);
	_uint		m_iNumPolygons = 0;
	_vec3*		m_pPositions = nullptr;
	void*		m_pIndices = nullptr;

public:
	virtual void		Free();
};

