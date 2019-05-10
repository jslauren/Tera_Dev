#pragma once
const unsigned int g_iToolWinCX = 800;
const unsigned int g_iToolWinCY = 600;

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