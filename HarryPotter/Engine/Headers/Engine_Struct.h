#pragma once

namespace Engine
{
	typedef struct tagObjectMeshData
	{
		_bool		bIsStaticMesh;	// 스태틱 메쉬인지 판별하기 위한 툴용 변수.
		_matrix		matWorld;		// 해당 오브젝트의 행렬 정보를 저장하기 위한 matrix
		wstring		strObjProtoTag;	// Add_Object를 할 때 필요한 값 정보인 변수.
		wstring		strLayerTag;	// 툴의 트리컨트롤을 위한 변수. 여기서 strObjName도 제조한다.
		wstring		strComProtoTag;	// 이 또한 툴용 변수이다.
		wstring		strFullPath;

	}OBJECTMESHDATA;

	typedef struct tagTerrainData
	{
		_tchar*		pImgPath;
		int			iNumVtxX;
		int			iNumVtxZ;
		float		fInterval;
		float		fDetail;

	}TERRAINDATA;

	typedef struct tagMeshTexture
	{
		enum TYPE { TYPE_DIFFUSE, TYPE_NORMAL, TYPE_SPECULAR, TYPE_END };

		LPDIRECT3DTEXTURE9		pTextures[TYPE_END];

	}MESHTEXTURE;

	typedef struct tagD3DXMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPD3DXMESH			pOriginalMesh;
		MESHTEXTURE*		pMeshTexture;
		unsigned long		dwNumBones;
		D3DXMATRIX*			pRenderingMatrices = nullptr;
		D3DXMATRIX*			pOffsetMatrices = nullptr;
		D3DXMATRIX**		ppCombinedTransformationMatrices = nullptr;

	}D3DXMESHCONTAINER_DERIVED, *PD3DXMESHCONTAINER_DERIVED;

	typedef struct tagD3DXFrame_Derived : public D3DXFRAME
	{
		//  나`s TransformationMatrix * 부모`s CombinedTransformationMatrix
		D3DXMATRIX			CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagVertex_Cube_Texture
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		D3DXVECTOR3			vTexUV; // D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0)

	}VTXCUBETEX;

	typedef struct tagVertex_ViewPort
	{
		D3DXVECTOR4			vPosition; // D3DFVF_XYZRHW
		D3DXVECTOR2			vTexUV; // D3DFVF_TEX1

	}VTXVIEWPORT;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		D3DXVECTOR2			vTexUV; // D3DFVF_TEX1

	}VTXTEX;

	typedef struct tagVertex
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ

	}VTX;

	typedef struct tagVertex_Normal_Texture
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTexUV; // D3DFVF_TEX1

	}VTXNORTEX;

	typedef struct tagVertex_Color
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		unsigned long		dwColor; // D3DFVF_DIFFUSE	

	}VTXCOL;

	typedef struct tagIndex16
	{
		unsigned short	_1, _2, _3;

	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long	_1, _2, _3;

	}INDEX32;


}

