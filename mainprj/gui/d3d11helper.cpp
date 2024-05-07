#include "d3d11helper.h"
#include <iostream>

#pragma comment(lib, "d3d11.lib")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  d3d11 & winapi helper functions
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace My::Gui
{

	// Helper functions
	bool D3d11Face::CreateDeviceD3D(HWND hWnd)
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
		if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
			res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
		if (res != S_OK)
			return false;

		CreateRenderTarget();
		return true;
	}

	void D3d11Face::CleanupDeviceD3D()
	{
		CleanupRenderTarget();
		
		if (g_pSwapChain) 
		{ 
			g_pSwapChain->Release(); 
			g_pSwapChain = nullptr; 
		}

		if (g_pd3dDeviceContext) 
		{ 
			g_pd3dDeviceContext->Release(); 
			g_pd3dDeviceContext = nullptr; 
		}

		if (g_pd3dDevice) 
		{ 
			g_pd3dDevice->Release(); 
			g_pd3dDevice = nullptr; 
		}
	}

	void D3d11Face::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer{};
		g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (pBackBuffer)
		{
			g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
			pBackBuffer->Release();
		}
	}

	void D3d11Face::CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) 
		{ 
			g_mainRenderTargetView->Release(); 
			g_mainRenderTargetView = nullptr; 
		}
	}

	

}
