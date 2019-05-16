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

#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;
#ifdef _TOOL
#else
#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG 
#endif // _TOOL

// 내가 제작한 헤더파일
#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"

// 외부 헤더파일

using namespace Engine;