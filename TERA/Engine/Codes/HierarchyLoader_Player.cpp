#include "..\Headers\HierarchyLoader_Player.h"

CHierarchyLoader_Player::CHierarchyLoader_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

STDMETHODIMP CHierarchyLoader_Player::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*	pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewFrame->Name, Name)))
			return E_FAIL;
	}

	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->CombinedTransformationMatrix);

	*ppNewFrame = pNewFrame;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader_Player::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	*ppNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(*ppNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));
	
	return NOERROR;
}

STDMETHODIMP CHierarchyLoader_Player::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader_Player::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	if (pMeshContainerToFree != nullptr)
		Safe_Delete(pMeshContainerToFree);

	return NOERROR;
}

HRESULT CHierarchyLoader_Player::Ready_HierarchyLoader(const _tchar * pFilePath)
{
	m_pFilePath = pFilePath;

	return NOERROR;
}

_ulong CHierarchyLoader_Player::AddRef()
{
	return _ulong(++m_dwRefCnt);
}

_ulong CHierarchyLoader_Player::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}

HRESULT CHierarchyLoader_Player::Allocate_Name(char ** ppSour, const char * pDest)
{
	size_t iLength = strlen(pDest);

	*ppSour = new char[iLength + 1];
	ZeroMemory(*ppSour, sizeof(char) * iLength + 1);

	strcpy(*ppSour, pDest);

	return NOERROR;
}

HRESULT CHierarchyLoader_Player::Change_TextureFileName(_tchar * pFileName, const _tchar Text)
{
	_uint iLength = lstrlen(pFileName);

	for (size_t i = iLength; i >= 0; --i)
	{
		if (pFileName[i] == L'.')
		{
			pFileName[i - 1] = Text;
			break;
		}
	}

	return NOERROR;
}

CHierarchyLoader_Player * CHierarchyLoader_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath)
{
	CHierarchyLoader_Player* pInstance = new CHierarchyLoader_Player(pGraphic_Device);

	if (FAILED(pInstance->Ready_HierarchyLoader(pFilePath)))
	{
		_MSGBOX("CHierarchyLoader_Player Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyLoader_Player::Free()
{
	Safe_Release(m_pGraphic_Device);
}
