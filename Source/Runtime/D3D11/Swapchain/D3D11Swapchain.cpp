#include "D3D11Swapchain.h"

#include <Runtime/Window/WindowManager.h>
#include <Runtime/D3D11/Texture/D3D11TextureUtils.h>
#include <Runtime/D3D11/Instance/D3D11Instance.h>

namespace Hollow
{
	D3D11Swapchain::D3D11Swapchain(const SwapchainDesc& desc, D3D11Device* pDevice) : Swapchain(desc)
	{
		DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
		swapchainDesc.BufferCount = GetBufferCount();
		swapchainDesc.BufferDesc.Format = D3D11TextureUtils::GetDXTextureFormat(GetSwapchainFormat());
		swapchainDesc.BufferDesc.Width = WindowManager::GetInstance().GetMainWindow()->GetWindowSize().x;
		swapchainDesc.BufferDesc.Height = WindowManager::GetInstance().GetMainWindow()->GetWindowSize().y;
		swapchainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		swapchainDesc.OutputWindow = WindowManager::GetInstance().GetMainWindow()->GetWindowNativeHandle();
		swapchainDesc.SampleDesc.Count = GetSampleCount();
		swapchainDesc.SampleDesc.Quality = GetSampleCount() > 1 ? 1 : 0;
		swapchainDesc.Windowed = GetWindowed();
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	
		DEV_ASSERT(SUCCEEDED(static_cast<D3D11Instance*>(desc.pInstance.get())->GetD3DInstance()->CreateSwapChain(pDevice->GetD3DDevice().Get(),
			&swapchainDesc, mD3DSwapchain.GetAddressOf())), "D3D11Swapchain", "Failed to create D3D11Swapchain");

		CORE_LOG(HE_VERBOSE, "D3D11Swapchain", "Operation is successful.");
	}

	void D3D11Swapchain::PresentCore()
	{
		mD3DSwapchain->Present(GetVSync(), 0);
	}

	void D3D11Swapchain::ResizeCore(Vector2u newSize)
	{
		ID3D11Device* pDevice = nullptr;
		DEV_ASSERT(SUCCEEDED(mD3DSwapchain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)), "D3D11Swapchain", "Failed to get D3D11Device from DXGISwapchain");

		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		mD3DSwapchain->GetDesc(&swapchainDesc);

		// Resize swapchain
		DEV_ASSERT(SUCCEEDED(mD3DSwapchain->ResizeBuffers(swapchainDesc.BufferCount, newSize.x, newSize.y, swapchainDesc.BufferDesc.Format,
			swapchainDesc.Flags)), "D3D11Swapchain", "Failed to resize DXGISwapchain");
		DEV_LOG(HE_VERBOSE, "Resized Swapchain to size: { %d, %d }", newSize.x, newSize.y);

		pDevice->Release();
	}
}
