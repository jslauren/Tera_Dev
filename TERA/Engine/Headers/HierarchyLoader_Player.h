#pragma once
#include "Engine_Defines.h"

_BEGIN(Engine)

class CHierarchyLoader_Player final : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader_Player(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CHierarchyLoader_Player() = default;
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
public:
	HRESULT Ready_HierarchyLoader(const _tchar* pFilePath);
public:
	_ulong	AddRef(); // 레퍼런스 카운트를 증가시킨다.
	_ulong	Release(); // 레퍼런스 카운트를 감소시킨다. + 레퍼런스카운트가 0인경우 삭제한다.
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	const _tchar*		m_pFilePath = nullptr;
private:
	_ulong	m_dwRefCnt = 0;
private:
	HRESULT Allocate_Name(char** ppSour, const char* pDest);
	HRESULT Change_TextureFileName(_tchar* pFileName, const _tchar Text);
public:
	static CHierarchyLoader_Player*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual void					Free();
};

_END