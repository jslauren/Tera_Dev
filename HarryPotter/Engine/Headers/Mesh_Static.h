#pragma once
#include "Component.h"

_BEGIN(Engine)

class _DLL_EXPORTS CMesh_Static final : public CComponent
{
private:
	explicit CMesh_Static(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMesh_Static(const CMesh_Static& rhs);
	virtual ~CMesh_Static() = default;
public:	// Getter
	_uint				Get_NumMaterials() const { return m_dwNumMaterials; }
	const D3DXMATERIAL* Get_Materials() const {	return m_pSubsetMaterial; }
public:
	HRESULT Ready_Mesh_Static(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT Render_Mesh(_uint iSubSetID);
	HRESULT SetTexture_OnShader(LPD3DXEFFECT pEffect, _uint iSubsetID, const char* pConstantName, MESHTEXTURE::TYPE eType);
private:
	// �޽ø� ��ǥ�ϴ� ��ü.
	LPD3DXMESH			m_pMesh = nullptr;

	// �޽��� ���������� ����.
	LPD3DXBUFFER		m_pMaterials = nullptr;
	LPD3DXBUFFER		m_pAdjacency = nullptr;
	_ulong				m_dwNumMaterials = 0;
private: // �����迭. �迭�� ���� == ������� ����.
	D3DXMATERIAL*		m_pSubsetMaterial = nullptr;
	MESHTEXTURE*		m_pMeshTexture = nullptr;
private:
	HRESULT Change_TextureFileName(_tchar* pFileName, const _tchar Text);
public:
	static CMesh_Static*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END