#pragma once
#include "Component.h"

_BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class _DLL_EXPORTS CMesh_Dynamic_Parts final : public CComponent
{
private:
	explicit CMesh_Dynamic_Parts(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic_Parts(const CMesh_Dynamic_Parts& rhs);
	virtual ~CMesh_Dynamic_Parts() = default;
public:
	_uint		Get_NumMeshContainer() { return m_MeshContainers.size(); }
	_uint		Get_NumSubSet(_uint iMeshContainerID) { return m_MeshContainers[iMeshContainerID]->NumMaterials; }
	D3DXFRAME*	GetRootFrame() { return m_pRootFrame; }
public:
	HRESULT Ready_Mesh_Dynamic(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT Render_Mesh(_uint iMeshContainerID, _uint iSubSetID);
	HRESULT Update_SkinnedMesh(_uint iMeshContainerID);
	HRESULT SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iMeshContainerID, _uint iSubsetID, const char* pConstantName, MESHTEXTURE::TYPE eType);
public:
	HRESULT ChangePivot(_vec3 vState, _float fDegree);
private:
	CHierarchyLoader*		m_pLoader = nullptr;
	D3DXFRAME*				m_pRootFrame = nullptr;
	vector<D3DXMESHCONTAINER_DERIVED*>	m_MeshContainers;
	_matrix					m_matPivot;

private:
	HRESULT SetUp_CombinedMatrixPointer(D3DXFRAME* pFrame);

public:
	static CMesh_Dynamic_Parts*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END