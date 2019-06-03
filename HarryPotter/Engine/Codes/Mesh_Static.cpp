#include "..\Headers\Mesh_Static.h"

CMesh_Static::CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CMesh_Static::CMesh_Static(const CMesh_Static & rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pMaterials(rhs.m_pMaterials)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_dwNumMaterials(rhs.m_dwNumMaterials)
	, m_pSubsetMaterial(rhs.m_pSubsetMaterial)
	, m_pMeshTexture(rhs.m_pMeshTexture)
{
	m_pMesh->AddRef();
	m_pMaterials->AddRef();
	m_pAdjacency->AddRef();

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		Safe_AddRef(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_DIFFUSE]);
		Safe_AddRef(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_NORMAL]);
		Safe_AddRef(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_SPECULAR]);
	}
}

HRESULT CMesh_Static::Ready_Mesh_Static(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	LPD3DXMESH			pMesh = nullptr;

	if (FAILED(D3DXLoadMeshFromX(szFullPath, 0, m_pGraphic_Device, &m_pAdjacency, &m_pMaterials, nullptr, &m_dwNumMaterials, &pMesh)))
		return E_FAIL;

	m_pMeshTexture = new MESHTEXTURE[m_dwNumMaterials];
	ZeroMemory(m_pMeshTexture, sizeof(MESHTEXTURE) * m_dwNumMaterials);

	m_pSubsetMaterial = new D3DXMATERIAL[m_dwNumMaterials];
	memcpy(m_pSubsetMaterial, m_pMaterials->GetBufferPointer(), sizeof(D3DXMATERIAL) * m_dwNumMaterials);


	if (false == (D3DFVF_NORMAL & pMesh->GetFVF()))
	{
		// 메시를 복제한다.
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, m_pGraphic_Device, &m_pMesh)))
			return E_FAIL;

		if (FAILED(D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer())))
			return E_FAIL;
	}
	else
	{
		// 메시를 구성하는 정점구조체의 멤버변수의 정보를 담기위한 구조체배열
		D3DVERTEXELEMENT9			Element[MAX_FVF_DECL_SIZE] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		if (FAILED(pMesh->CloneMesh(pMesh->GetOptions(), Element, m_pGraphic_Device, &m_pMesh)))
			return E_FAIL;
	}

	Safe_Release(pMesh);

	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		lstrcpy(szFullPath, pFilePath);

		_tchar		szFileName[MAX_PATH] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pSubsetMaterial[i].pTextureFilename, strlen(m_pSubsetMaterial[i].pTextureFilename)
			, szFileName, MAX_PATH);

		lstrcat(szFullPath, szFileName);

		// For.DiffuseMap
		if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_DIFFUSE])))
			return E_FAIL;

		//// For.NormalMap
		//Change_TextureFileName(szFullPath, L'N');
		//D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_NORMAL]);

		//// For.SpecularMap
		//Change_TextureFileName(szFullPath, L'S');
		//D3DXCreateTextureFromFile(m_pGraphic_Device, szFullPath, &m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_SPECULAR]);
	}

	return NOERROR;
}

HRESULT CMesh_Static::Render_Mesh(_uint iSubSetID)
{
	if (nullptr == m_pMesh)
		return E_FAIL;

	m_pMesh->DrawSubset(iSubSetID);

	return NOERROR;
}
HRESULT CMesh_Static::SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iSubsetID, const char * pConstantName, MESHTEXTURE::TYPE eType)
{
	if (nullptr == pEffect ||
		nullptr == m_pMeshTexture)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_pMeshTexture[iSubsetID].pTextures[eType]);

	return NOERROR;
}
HRESULT CMesh_Static::Change_TextureFileName(_tchar* pFileName, _tchar Text)
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

CMesh_Static * CMesh_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh_Static* pInstance = new CMesh_Static(pGraphic_Device);

	if (FAILED(pInstance->Ready_Mesh_Static(pFilePath, pFileName)))
	{
		_MSGBOX("CMesh_Static Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMesh_Static::Clone(void* pArg)
{
	return new CMesh_Static(*this);
}

void CMesh_Static::Free()
{
	for (size_t i = 0; i < m_dwNumMaterials; ++i)
	{
		Safe_Release(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_DIFFUSE]);
		Safe_Release(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_NORMAL]);
		Safe_Release(m_pMeshTexture[i].pTextures[MESHTEXTURE::TYPE_SPECULAR]);
	}

	if (false == m_isClone)
	{
		Safe_Delete_Array(m_pSubsetMaterial);
		Safe_Delete_Array(m_pMeshTexture);
	}

	Safe_Release(m_pMaterials);
	Safe_Release(m_pAdjacency);
	_ulong dwRefCnt = Safe_Release(m_pMesh);

	CComponent::Free();
}
