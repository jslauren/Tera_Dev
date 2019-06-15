#include "..\Headers\Mesh_Dynamic_Bone.h"
#include "HierarchyLoader_Player.h"
#include "AnimationCtrl.h"

CMesh_Dynamic_Bone::CMesh_Dynamic_Bone(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Dynamic_Bone::CMesh_Dynamic_Bone(const CMesh_Dynamic_Bone & rhs)
	: CComponent(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_matPivot(rhs.m_matPivot)
	, m_pAniCtrl(CAnimationCtrl::Create(*rhs.m_pAniCtrl))
{
	Safe_AddRef(m_pLoader);
}

const _uint & CMesh_Dynamic_Bone::Get_NowPlayAniIndex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_NowPlayAniIndex();
}

const _double & CMesh_Dynamic_Bone::Get_CurrentTrackPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_CurretTrackPos();
}

const _double & CMesh_Dynamic_Bone::Get_Period()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_Period();
}

HRESULT CMesh_Dynamic_Bone::Ready_Mesh_Dynamic(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader_Player::Create(m_pGraphic_Device, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER			pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphic_Device, m_pLoader, nullptr, &m_pRootFrame, &pAniCtrl)))
		return E_FAIL;

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	Safe_Release(pAniCtrl);

	//D3DXMatrixRotationX(&m_matPivot, D3DXToRadian(180.0f));
	D3DXMatrixIdentity(&m_matPivot);

	if (FAILED(Update_CombinedTransformationMatrix(m_pRootFrame, &m_matPivot, m_pHeadFrame, m_pBodyFrame, m_pHandFrame, m_pLegFrame, m_pTailFrame)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMesh_Dynamic_Bone::SetUp_AnimationSet(_uint iIndex, _float fAniSpeed)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->SetUp_AnimationSet(iIndex, fAniSpeed);

	return NOERROR;
}

HRESULT CMesh_Dynamic_Bone::SetUp_AnimationSet(const char * pName)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->SetUp_AnimationSet(pName);

	return NOERROR;
}

HRESULT CMesh_Dynamic_Bone::Play_Animation(const _float & fTimeDelta, const _float fAniSpeed)
{
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	m_pAniCtrl->Play_Animation(fTimeDelta * fAniSpeed);

	Update_CombinedTransformationMatrix(m_pRootFrame, &m_matPivot, m_pHeadFrame, m_pBodyFrame, m_pHandFrame, m_pLegFrame, m_pTailFrame);

	return NOERROR;
}

_bool CMesh_Dynamic_Bone::IsAnimationEnded(_double fCtrlEndTime)
{
	return m_pAniCtrl->IsAnimationEnded(fCtrlEndTime);
}

HRESULT CMesh_Dynamic_Bone::ChangePivot(_vec3 vState, _float fDegree)
{
	_matrix matPivot;
	D3DXMatrixIdentity(&matPivot);

	D3DXMatrixRotationAxis(&matPivot, &vState, D3DXToRadian(fDegree));

	m_matPivot *= matPivot;

	return NOERROR;
}

HRESULT CMesh_Dynamic_Bone::Update_CombinedTransformationMatrix(D3DXFRAME * pFrame, const D3DXMATRIX * pParentMatrix, D3DXFRAME* pSub1, D3DXFRAME* pSub2, D3DXFRAME* pSub3, D3DXFRAME* pSub4, D3DXFRAME* pSub5)
{
	D3DXFRAME_DERIVED*		pFrame_Derived = (D3DXFRAME_DERIVED*)pFrame;

	D3DXFRAME_DERIVED*		pFrame_Derived1 = (D3DXFRAME_DERIVED*)pSub1;
	D3DXFRAME_DERIVED*		pFrame_Derived2 = (D3DXFRAME_DERIVED*)pSub2;
	D3DXFRAME_DERIVED*		pFrame_Derived3 = (D3DXFRAME_DERIVED*)pSub3;
	D3DXFRAME_DERIVED*		pFrame_Derived4 = (D3DXFRAME_DERIVED*)pSub4;
	D3DXFRAME_DERIVED*		pFrame_Derived5 = (D3DXFRAME_DERIVED*)pSub5;

	D3DXFRAME_DERIVED*		pFrame_Derived_FirstChild1 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_FirstChild2 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_FirstChild3 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_FirstChild4 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_FirstChild5 = nullptr;

	D3DXFRAME_DERIVED*		pFrame_Derived_Sibling1 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_Sibling2 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_Sibling3 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_Sibling4 = nullptr;
	D3DXFRAME_DERIVED*		pFrame_Derived_Sibling5 = nullptr;

	pFrame_Derived->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;

	if (nullptr != pFrame_Derived1)
	{
		pFrame_Derived1->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;
		pFrame_Derived_FirstChild1 = (D3DXFRAME_DERIVED*)pFrame_Derived1->pFrameFirstChild;
		pFrame_Derived_Sibling1 = (D3DXFRAME_DERIVED*)pFrame_Derived1->pFrameSibling;
	}
	if (nullptr != pFrame_Derived2)
	{
		pFrame_Derived2->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;
		pFrame_Derived_FirstChild2 = (D3DXFRAME_DERIVED*)pFrame_Derived2->pFrameFirstChild;
		pFrame_Derived_Sibling2 = (D3DXFRAME_DERIVED*)pFrame_Derived2->pFrameSibling;
	}
	if (nullptr != pFrame_Derived3)
	{
		pFrame_Derived3->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;
		pFrame_Derived_FirstChild3 = (D3DXFRAME_DERIVED*)pFrame_Derived3->pFrameFirstChild;
		pFrame_Derived_Sibling3 = (D3DXFRAME_DERIVED*)pFrame_Derived3->pFrameSibling;
	}
	if (nullptr != pFrame_Derived4)
	{
		pFrame_Derived4->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;
		pFrame_Derived_FirstChild4 = (D3DXFRAME_DERIVED*)pFrame_Derived4->pFrameFirstChild;
		pFrame_Derived_Sibling4 = (D3DXFRAME_DERIVED*)pFrame_Derived4->pFrameSibling;
	}
	if (nullptr != pFrame_Derived5)
	{
		pFrame_Derived5->CombinedTransformationMatrix = pFrame_Derived->TransformationMatrix * *pParentMatrix;
		pFrame_Derived_FirstChild5 = (D3DXFRAME_DERIVED*)pFrame_Derived5->pFrameFirstChild;
		pFrame_Derived_Sibling5 = (D3DXFRAME_DERIVED*)pFrame_Derived5->pFrameSibling;
	}

	if (nullptr != pFrame_Derived->pFrameFirstChild)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameFirstChild, &pFrame_Derived->CombinedTransformationMatrix, pFrame_Derived_FirstChild1, pFrame_Derived_FirstChild2, pFrame_Derived_FirstChild3, pFrame_Derived_FirstChild4, pFrame_Derived_FirstChild5);
	}

	if (nullptr != pFrame_Derived->pFrameSibling)
	{
		Update_CombinedTransformationMatrix(pFrame_Derived->pFrameSibling, pParentMatrix, pFrame_Derived_Sibling1, pFrame_Derived_Sibling2, pFrame_Derived_Sibling3, pFrame_Derived_Sibling4, pFrame_Derived_Sibling5);
	}

	return NOERROR;
}

CMesh_Dynamic_Bone * CMesh_Dynamic_Bone::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Dynamic_Bone* pInstance = new CMesh_Dynamic_Bone(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Dynamic(pFilePath, pFileName)))
	{
		_MSGBOX("CMesh_Dynamic_Bone Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Dynamic_Bone::Clone(void * pArg)
{
	return new CMesh_Dynamic_Bone(*this);
}

void CMesh_Dynamic_Bone::Free()
{
	if (false == m_isClone)
		m_pLoader->DestroyFrame(m_pRootFrame);

	Safe_Release(m_pLoader);
	Safe_Release(m_pAniCtrl);

	CComponent::Free();
}
