#pragma once

namespace Engine
{
	typedef struct tagVertex_Color
	{
		D3DXVECTOR3			vPosition; // D3DFVF_XYZ
		unsigned long		dwColor; // D3DFVF_DIFFUSE	
	}VTXCOL;

	typedef struct tagIndex16
	{
		unsigned short _1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned short _1, _2, _3;
	}INDEX32;

}

