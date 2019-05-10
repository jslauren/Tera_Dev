#pragma once

const float g_fRate60 = 1.f / 60.0f;
const float g_fRate100 = 1.f / 100.0f;
const float g_fRateImmediate = 0.0f;

const unsigned int g_iToolWinCX = 800;
const unsigned int g_iToolWinCY = 600;

enum TOOLSCENEID 
{
	TOOL_SCENE_STATIC, 
	TOOL_SCENE_TERRAIN,
	TOOL_SCENE_MESH,
	TOOL_SCENE_CAMERA,
	TOOL_SCENE_EFFECT,
	TOOL_SCENE_END 
};


enum TOOLID
{
	TOOL_STATIC,
	TOOL_TERRAIN,
	TOOL_OBJ,
	TOOL_PROTO,
	TOOL_END
};

extern HWND g_ToolhWnd;

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
