#pragma once

#include <Windows.h>
#include <assert.h>

// STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <thread>
#include <mutex>
using namespace std;

// Direct3D
#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcommon.h>
#include <D3D11Shader.h>
#include <DXGI.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <d3dx11effect.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Effects11d.lib")

// ImGui
#include <ImGui_New/imgui.h>
#include <ImGui_New/imgui_impl_dx11.h>
#include <ImGui_New/imgui_impl_win32.h>
#pragma comment(lib, "ImGui_New/imgui.lib")

// DirectXTex
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

#define Check(hr) {assert(SUCCEEDED(hr));}
#define Super __super

#define SafeRelease(p) { if(p) {(p)->Release(); (p) = NULL; } }
#define SafeDelete(p) { if(p) { delete (p); (p) = NULL; } }
#define SafeDeleteArray(p) { if(p) { delete[] (p); (p) = NULL; } }

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;
typedef D3DXPLANE Plane;

#include "Systems/D3D.h"
#include "Systems/Keyboard.h"