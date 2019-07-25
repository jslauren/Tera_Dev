#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 4005)

#include <process.h>

const float g_fRate_60 = 1.f / 60.0f;
const float g_fRate_100 = 1.f / 100.0f;
const float g_fRate_Immediate = 0.0f;

const unsigned int g_iWinCX = 1280;
const unsigned int g_iWinCY = 720;

const unsigned int g_iWinViewCX = 980;
const unsigned int g_iWinViewCY = 701;

enum SCENEID { SCENE_STATIC, SCENE_LOGO, SCENE_STAGE, SCENE_DRAGON, SCENE_BOSS, SCENE_LOADING, SCENE_END };

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif

//namespace Client {}
//
//using namespace Client;