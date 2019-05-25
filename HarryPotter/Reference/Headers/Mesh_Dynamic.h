#pragma once
#include "Component.h"

_BEGIN(Engine)

class CAnimationCtrl;
class CHierarchyLoader;
class _DLL_EXPORTS CMesh_Dynamic final : public CComponent
{
private:
	explicit CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Dynamic(const CMesh_Dynamic& rhs);
	virtual ~CMesh_Dynamic() = default;
public:
	_uint	Get_NumMeshContainer() { return m_MeshContainers.size(); }
	_uint	Get_NumSubSet(_uint iMeshContainerID) { return m_MeshContainers[iMeshContainerID]->NumMaterials; }
	const D3DXFRAME_DERIVED* Get_FrameDesc(const char* pFrameName) { return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName); }
public:
	HRESULT Ready_Mesh_Dynamic(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT Render_Mesh(_uint iMeshContainerID, _uint iSubSetID);
	HRESULT Update_SkinnedMesh(_uint iMeshContainerID);
	HRESULT SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iMeshContainerID, _uint iSubsetID, const char* pConstantName, MESHTEXTURE::TYPE eType);
	HRESULT SetUp_AnimationSet(_uint iIndex);
	HRESULT SetUp_AnimationSet(const char* pName);
	HRESULT Play_Animation(const _float& fTimeDelta);
public:
	HRESULT ChangePivot(_vec3 vState, _float fDegree);
private:
	CHierarchyLoader*		m_pLoader = nullptr;
	D3DXFRAME*				m_pRootFrame = nullptr;
	vector<D3DXMESHCONTAINER_DERIVED*>	m_MeshContainers;
	CAnimationCtrl*			m_pAniCtrl = nullptr;
	_matrix					m_matPivot;
private:
	HRESULT Update_CombinedTransformationMatrix(D3DXFRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	HRESULT SetUp_CombinedMatrixPointer(D3DXFRAME* pFrame);

public:
	static CMesh_Dynamic*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone();
	virtual void			Free();

};

_END