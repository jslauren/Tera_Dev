#include "..\Headers\Mesh_Dynamic.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"

CMesh_Dynamic::CMesh_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Dynamic::CMesh_Dynamic(const CMesh_Dynamic & rhs)
	: CComponent(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_matPivot(rhs.m_matPivot)
	, m_pAniCtrl(CAnimationCtrl::Create(*rhs.m_pAniCtrl))
{
	Safe_AddRef(m_pLoader);
}

const _uint & CMesh_Dynamic::Get_NowPlayAniIndex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_NowPlayAniIndex(); 
}

const _double & CMesh_Dynamic::Get_CurrentTrackPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_CurretTrackPos();
}

const _double & CMesh_Dynamic::Get_Period()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_Period();
}

HRESULT CMesh_Dynamic::Ready_Mesh_Dynamic(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphic_Device, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER			pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, &pAniCtrl)))
		return E_FAIL;

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);
 	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	Safe_Release(pAniCtrl);

	D3DXMatrixIdentity(&m_matPivot);
	D3DXMatrixRotationY(&m_matPivot, D3DXToRadian(-90.0f));

	if (FAILED(Update_CombinedTransformationMatrix(m_pRootFrame, &m_matPivot)))
		return E_FAIL;

	if (FAILED(SetUp_CombinedMatrixPointer(m_pRootFrame)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Dynamic::Render_Mesh(_uint iMeshContainerID, _uint iSubSetID)
{
	m_MeshContainers[iMeshContainerID]->MeshData.pMesh->DrawSubset(iSubSetID);

	return NOERROR;
}

HRESULT CMesh_Dynamic::Update_SkinnedMesh(_uint iMeshContainerID)
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

HRESULT CMesh_Dynamic::SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iMeshContainerID, _uint iSubsetID, const char * pConstantName, MESHTEXTURE::TYPE eType)
{
	pEffect->SetTexture(pConstantName, m_MeshContainers[iMeshContainerID]->pMeshTexture[iSubsetID].pTextures[eType]);

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_AnimationSet(_uint iIndex, _float fAniSpeed)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->SetUp_AnimationSet(iIndex, fAniSpeed);

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_AnimationSet(const char * pName)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->SetUp_AnimationSet(pName);

	return NOERROR;
}

HRESULT CMesh_Dynamic::Play_Animation(const _float & fTimeDelta, const _float fAniSpeed)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->Play_Animation(fTimeDelta * fAniSpeed);

	// 애니메이션에 들어가있는 이동 값을 조져버리는 구문이다.
	m_pRootFrame->pFrameFirstChild->TransformationMatrix.m[3][0] = 0;
	m_pRootFrame->pFrameFirstChild->TransformationMatrix.m[3][2] = 0;

	Update_CombinedTransformationMatrix(m_pRootFrame, &m_matPivot);

	return NOERROR;
}

_bool CMesh_Dynamic::IsAnimationEnded(_double fCtrlEndTime)
{
	return m_pAniCtrl->IsAnimationEnded(fCtrlEndTime);
}

HRESULT CMesh_Dynamic::ChangePivot(_vec3 vState, _float fDegree)
{
	_matrix matPivot;
	D3DXMatrixIdentity(&matPivot);

	D3DXMatrixRotationAxis(&matPivot, &vState, D3DXToRadian(fDegree));

	m_matPivot *= matPivot;

	return NOERROR;
}


HRESULT CMesh_Dynamic::Update_CombinedTransformationMatrix(D3DXFRAME * pFrame, const D3DXMATRIX* pParentMatrix)
{
	D3DXFRAME_DERIVED*		pFrame_Derived = (D3DXFRAME_DERIVED*)pFrame;

	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;

	if (nullptr != pFrame_Derived->pFrameFirstChild)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameFirstChild, &pFrame_Derived->CombinedTransformationMatrix);
	}

	if (nullptr != pFrame_Derived->pFrameSibling)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameSibling, pParentMatrix);
	}

	return NOERROR;
}

HRESULT CMesh_Dynamic::SetUp_CombinedMatrixPointer(D3DXFRAME* pFrame)
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

CMesh_Dynamic * CMesh_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic* pInstance = new CMesh_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Dynamic(pFilePath, pFileName)))
	{
		_MSGBOX("CMesh_Dynamic Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Dynamic::Clone(void* pArg)
{
	return new CMesh_Dynamic(*this);
}

void CMesh_Dynamic::Free()
{
	if (false == m_isClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
	}

	Safe_Release(m_pLoader);
	Safe_Release(m_pAniCtrl);

	m_MeshContainers.clear();

	CComponent::Free();
}
