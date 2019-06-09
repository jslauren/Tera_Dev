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
	void	Set_Terrain_Data();
	void	Take_Out_Terrain_Data_In_Map();

public:
	// Terrain_Vertex_Variable
	D3DFORMAT	m_Format = D3DFORMAT(0);
	_uint		m_iNumPolygons = 0;
	_vec3*		m_pPositions = nullptr;
	void*		m_pIndices = nullptr;

	// For.Terrain_Data I/O
	_tchar*		m_pImgPath;
	_int		m_iNumVtxX;
	_int		m_iNumVtxZ;
	_float		m_fInterval;
	_float		m_fDetail;

	// For.Mesh_Data I/O
	_bool		m_bIsStaticMesh;
	_matrix		m_matWorld;
	wstring		m_strObjProtoTag;
	wstring		m_strLayerTag;
	wstring		m_strComProtoTag;
	wstring		m_strFullPath;

	// map<wstring, vector<OBJECTMESHDATA>>의 First 인덱스로 사용하기 위한 변수.
	CString		m_strObjName;

	_int		m_iSelectTab;

public:
	map<int, TERRAINDATA*>					m_MapTerrainData;
	map<CString, vector<OBJECTMESHDATA>>	m_MapMeshData;

public:
	virtual void		Free();
};

