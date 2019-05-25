#include "..\Headers\HierarchyLoader.h"

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

// 엑스파일로부터 기초적인 정보를 로드하여 CreateFrame함수를 호출하며 이자를 전달하고. 
// ppNewFrame에 유저가 할당한 뼈의 주소를 넣어달라고 한다.
STDMETHODIMP CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	D3DXFRAME_DERIVED*	pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewFrame->Name, Name)))
			return E_FAIL;
	}

	*ppNewFrame = pNewFrame;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	if (nullptr != Name)
	{
		if (FAILED(Allocate_Name(&pNewMeshContainer->Name, Name)))
			return E_FAIL;
	}

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;
	Safe_AddRef(pMesh);

	_ulong num = pMesh->GetNumFaces() * 3;
	pNewMeshContainer->pAdjacency = new _ulong[num];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * num);

	_ulong	dwFVF = pMesh->GetFVF();

	if (dwFVF & D3DFVF_NORMAL)
	{
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphic_Device, &pNewMeshContainer->MeshData.pMesh)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphic_Device, &pNewMeshContainer->MeshData.pMesh)))
			return E_FAIL;

		if (FAILED(D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency)))
			return E_FAIL;
	}

	Safe_Release(pMesh);

	pNewMeshContainer->NumMaterials = NumMaterials == 0 ? 1 : NumMaterials;
	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		pNewMeshContainer->pMeshTexture = new MESHTEXTURE[pNewMeshContainer->NumMaterials];

		for (size_t i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[MAX_PATH] = L"";
			_tchar		szFileName[MAX_PATH] = L"";

			lstrcpy(szFullPath, m_pFilePath);

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, MAX_PATH);

			lstrcat(szFullPath, szFileName);

			// For.DiffuseTexture
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pNewMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_DIFFUSE])))
				return E_FAIL;

			// 기존 파일의 이름을 바꿔버린당께?? 조취가 필요하것어...

			//// For.NormalTexture
			//Change_TextureFileName(szFullPath, L'N');
			//D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pNewMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_NORMAL]);

			//// For.SpecularTexture
			//Change_TextureFileName(szFullPath, L'S');
			//D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &pNewMeshContainer->pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_SPECULAR]);

		}
	}

	pNewMeshContainer->pSkinInfo = pSkinInfo;
	Safe_AddRef(pNewMeshContainer->pSkinInfo);

	pNewMeshContainer->dwNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

	pNewMeshContainer->pRenderingMatrices = new D3DXMATRIX[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pRenderingMatrices, sizeof(D3DXMATRIX) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->pOffsetMatrices = new D3DXMATRIX[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pOffsetMatrices, sizeof(D3DXMATRIX) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->ppCombinedTransformationMatrices = new D3DXMATRIX*[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->ppCombinedTransformationMatrices, sizeof(D3DXMATRIX*) * pNewMeshContainer->dwNumBones);

	for (size_t i = 0; i < pNewMeshContainer->dwNumBones; ++i)
		pNewMeshContainer->pOffsetMatrices[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

	if (FAILED(pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(), pNewMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphic_Device, &pNewMeshContainer->pOriginalMesh)))
		return E_FAIL;

	*ppNewMeshContainer = pNewMeshContainer;

	return NOERROR;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
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

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	PD3DXMESHCONTAINER_DERIVED	pMeshContainer = (PD3DXMESHCONTAINER_DERIVED)pMeshContainerToFree;

	_ulong		dwRefCnt = 0;

	Safe_Delete_Array(pMeshContainer->Name);
	dwRefCnt = Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	dwRefCnt = Safe_Release(pMeshContainer->pSkinInfo);
	dwRefCnt = Safe_Release(pMeshContainer->pOriginalMesh);

	for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i)
	{
		// 나중에 TYPE_END로 변경해줘야함!!!!!
		for (size_t j = 0; j < MESHTEXTURE::TYPE_NORMAL; ++j)
			dwRefCnt = Safe_Release(pMeshContainer->pMeshTexture[i].pTextures[j]);
	}

	Safe_Delete_Array(pMeshContainer->pMeshTexture);
	Safe_Delete_Array(pMeshContainer->pOffsetMatrices);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
	Safe_Delete_Array(pMeshContainer->ppCombinedTransformationMatrices);

	Safe_Delete(pMeshContainerToFree);

	return NOERROR;
}

HRESULT CHierarchyLoader::Ready_HierarchyLoader(const _tchar * pFilePath)
{
	m_pFilePath = pFilePath;

	return NOERROR;
}

_ulong CHierarchyLoader::AddRef()
{
	return _ulong(++m_dwRefCnt);
}

unsigned long CHierarchyLoader::Release()
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

HRESULT CHierarchyLoader::Allocate_Name(char ** ppSour, const char * pDest)
{
	size_t iLength = strlen(pDest);

	*ppSour = new char[iLength + 1];
	ZeroMemory(*ppSour, sizeof(char) * iLength + 1);

	strcpy(*ppSour, pDest);

	return NOERROR;
}

HRESULT CHierarchyLoader::Change_TextureFileName(_tchar * pFileName, const _tchar Text)
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

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath)
{
	CHierarchyLoader* pInstance = new CHierarchyLoader(pGraphic_Device);

	if (FAILED(pInstance->Ready_HierarchyLoader(pFilePath)))
	{
		_MSGBOX("CHierarchyLoader Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyLoader::Free()
{
	Safe_Release(m_pGraphic_Device);
}
