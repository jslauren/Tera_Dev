#include "..\Headers\Mesh_Dynamic_Parts.h"
#include "HierarchyLoader.h"

CMesh_Dynamic_Parts::CMesh_Dynamic_Parts(LPDIRECT3DDEVICE9 pGraphic_Device)\
	: CComponent(pGraphic_Device)
{
}

CMesh_Dynamic_Parts::CMesh_Dynamic_Parts(const CMesh_Dynamic_Parts & rhs)
	: CComponent(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_matPivot(rhs.m_matPivot)
{
	Safe_AddRef(m_pLoader);
}

HRESULT CMesh_Dynamic_Parts::Ready_Mesh_Dynamic(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, nullptr)))
		return E_FAIL;

	//D3DXMatrixRotationX(&m_matPivot, D3DXToRadian(180.0f));
	D3DXMatrixIdentity(&m_matPivot);

	if (FAILED(SetUp_CombinedMatrixPointer(m_pRootFrame)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Dynamic_Parts::Render_Mesh(_uint iMeshContainerID, _uint iSubSetID)
{
	m_MeshContainers[iMeshContainerID]->MeshData.pMesh->DrawSubset(iSubSetID);

	return NOERROR;
}

HRESULT CMesh_Dynamic_Parts::Update_SkinnedMesh(_uint iMeshContainerID)
{
	if (m_MeshContainers.size() <= iMeshContainerID)
		return E_FAIL;

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = m_MeshContainers[iMeshContainerID];

	for (size_t i = 0; i < pMeshContainer->dwNumBones; ++i)
		pMeshContainer->pRenderingMatrices[i] = pMeshContainer->pOffsetMatrices[i] * *pMeshContainer->ppCombinedTransformationMatrices[i];

	void*	pSour, *pDest;

	pMeshContainer->pOriginalMesh->LockVertexBuffer(0, &pSour);
	pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDest);

	pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, nullptr, pSour, pDest);

	pMeshContainer->pOriginalMesh->UnlockVertexBuffer();
	pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	return NOERROR;
}

HRESULT CMesh_Dynamic_Parts::SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iMeshContainerID, _uint iSubsetID, const char * pConstantName, MESHTEXTURE::TYPE eType)
{
	pEffect->SetTexture(pConstantName, m_MeshContainers[iMeshContainerID]->pMeshTexture[iSubsetID].pTextures[eType]);

	return NOERROR;
}

HRESULT CMesh_Dynamic_Parts::ChangePivot(_vec3 vState, _float fDegree)
{
	_matrix matPivot;
	D3DXMatrixIdentity(&matPivot);

	D3DXMatrixRotationAxis(&matPivot, &vState, D3DXToRadian(fDegree));

	m_matPivot *= matPivot;

	return NOERROR;
}

HRESULT CMesh_Dynamic_Parts::SetUp_CombinedMatrixPointer(D3DXFRAME * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		m_MeshContainers.push_back(pMeshContainer);

		for (size_t i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

			pMeshContainer->ppCombinedTransformationMatrices[i] = &(pFindFrame->CombinedTransformationMatrix);
		}
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_CombinedMatrixPointer(pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_CombinedMatrixPointer(pFrame->pFrameFirstChild);

	return NOERROR;
}

CMesh_Dynamic_Parts * CMesh_Dynamic_Parts::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic_Parts* pInstance = new CMesh_Dynamic_Parts(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Dynamic(pFilePath, pFileName)))
	{
		_MSGBOX("CMesh_Dynamic_Parts Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Dynamic_Parts::Clone(void * pArg)
{
	return new CMesh_Dynamic_Parts(*this);
}

void CMesh_Dynamic_Parts::Free()
{
	if (false == m_isClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
	}

	Safe_Release(m_pLoader);

	m_MeshContainers.clear();

	CComponent::Free();
}
