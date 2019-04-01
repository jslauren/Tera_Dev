#pragma once

const float g_fRate_60 = 1.f / 60.0f;
const float g_fRate_100 = 1.f / 100.0f;
const float g_fRate_Immediate = 0.0f;

const unsigned int g_iWinCX = 800;
const unsigned int g_iWinCY = 600;

enum SCENEID { SCENE_STATIC, SCENE_LOGO, SCENE_STAGE, SCENE_END };

extern HWND g_hWnd;
