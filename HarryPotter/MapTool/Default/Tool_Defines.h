#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 4005)

#define	DIRECTINPUT_VERSION	0x0800

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma	comment (lib, "dinput8.lib")
#pragma	comment (lib, "dxguid.lib")

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;

// 이게 있으면 오류가 뻥뻥 터진다니까 ㅡㅡ?
//#ifdef _DEBUG 
//#ifndef DBG_NEW 
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//#endif 
//#endif  // _DEBUG 

const float g_fRate_60 = 1.f / 60.0f;
const float g_fRate_100 = 1.f / 100.0f;
const float g_fRate_Immediate = 0.0f;

const unsigned int g_iWinCX = 1600;
const unsigned int g_iWinCY = 900;

//const unsigned int g_iWinCX = 800;
//const unsigned int g_iWinCY = 600;

enum SCENEID { SCENE_STATIC, SCENE_TERRAIN, SCENE_MESH, SCENE_CAMERA, SCENE_EFFECT, SCENE_PROTO, SCENE_END };

extern HWND g_hWnd;
//extern HINSTANCE g_hInst;

typedef struct tagTexturePath
{
	std::wstring			wstrObjKey;
	std::wstring			wstrStateKey;
	std::wstring			wstrPath;
	int						iCount;

	tagTexturePath(void)
		: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
	{
	}
}IMGPATH;


//#pragma once
//const unsigned int g_iToolWinCX = 800;
//const unsigned int g_iToolWinCY = 600;
//
//enum TOOLID
//{
//	TOOL_STATIC,
//	TOOL_TERRAIN,
//	TOOL_OBJ,
//	TOOL_PROTO,
//	TOOL_END
//};
//
//extern HWND g_ToolhWnd;
//
//typedef struct tagTexturePath
//{
//	std::wstring			wstrObjKey;
//	std::wstring			wstrStateKey;
//	std::wstring			wstrPath;
//	int						iCount;
//
//	tagTexturePath(void)
//		: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
//	{
//	}
//}IMGPATH;