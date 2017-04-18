// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

//#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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

#include "DXMain.h"

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

//�߰� ���
#include "Define.h"
#include "Enum.h"
#include "Function.h"
#include "Struct.h"

//fbx ������ �̳��� �������
#include <fbxsdk.h>

using namespace std;
using namespace std::chrono;

//using namespace D2D1;
using namespace DirectX;
using namespace DirectX::PackedVector;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

//singleton
#include "GlobalValueManager.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "RenderContainerSeller.h"
#include "Debuger.h"
#include "FbxImporter.h"
#include "InputManager.h"
#include "DirectoryFinder.h"
#include "Exporter.h"
#include "Importer.h"
#include "Renderer.h"
#include "Updater.h"
#include "SceneMgr.h"
#include "CameraMgr.h"


#define		GLOBALVALUEMGR	CGlobalValueManager::GetInstance()
#define		TIMEMGR			CGameTimer::GetInstance()
#define		RESOURCEMGR		CResourceManager::GetInstance()
#define		RCSELLER		CRenderContainerSeller::GetInstance()
#define		DEBUGER			CDebuger::GetInstance()
#define		FBXIMPORTER		CFbxImporter::GetInstance()
#define		INPUTMGR		CInputManager::GetInstance()
#define		DIRECTORYFINDER	CDirectoryFinder::GetInstance()
#define		EXPORTER		CExporter::GetInstance()
#define		IMPORTER		CImporter::GetInstance()
#define		RENDERER		CRenderer::GetInstance()
#define		UPDATER			CUpdater::GetInstance()
#define		SCENEMGR		CSceneMgr::GetInstance()
#define		CAMMGR			CCameraMgr::GetInstance()

#define USE_ANIM
