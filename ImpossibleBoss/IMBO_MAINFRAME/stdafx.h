// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>

#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "atlcoll.h"

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <timeapi.h>
#include <ctime>
#include <time.h>
#include <timeapi.h>
#include <queue>
#include <unordered_map>
#include "winuser.h"                       //마우스 휠 이벤트 처리를 위한 헤더 함수

#ifndef WM_MOUSEWHEEL 

#define WM_MOUSEWHEEL 0x020A  //마우스 휠 이벤트 메세지 이름
#endif


// D3D11
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <D3DX11tex.h>
#include "DirectXTex.h"

// DirectX Math
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// C++11
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <stack>
#include <memory>

//추가 헤더
#include "Define.h"
#include "Enum.h"
#include "Function.h"
#include "Struct.h"


using namespace std;
using namespace std::chrono;

//using namespace D2D1;
using namespace DirectX;
using namespace DirectX::PackedVector;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//singleton
#include "GlobalValueManager.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "RenderContainerSeller.h"
#include "Debuger.h"
#include "InputManager.h"
#include "DirectoryFinder.h"
#include "Importer.h"
#include "Renderer.h"
#include "Updater.h"
#include "SceneMgr.h"
#include "CameraMgr.h"
#include "NetworkMgr.h"

#define		GLOBALVALUEMGR	CGlobalValueManager::GetInstance()
#define		TIMEMGR			CGameTimer::GetInstance()
#define		RESOURCEMGR		CResourceManager::GetInstance()
#define		RCSELLER		CRenderContainerSeller::GetInstance()
#define		DEBUGER			CDebuger::GetInstance()
#define		INPUTMGR		CInputManager::GetInstance()
#define		DIRECTORYFINDER	CDirectoryFinder::GetInstance()
#define		IMPORTER		CImporter::GetInstance()
#define		RENDERER		CRenderer::GetInstance()
#define		UPDATER			CUpdater::GetInstance()
#define		SCENEMGR		CSceneMgr::GetInstance()
#define		CAMMGR			CCameraMgr::GetInstance()
#define		NETWORKMGR		CNetworkManager::GetInstance()
