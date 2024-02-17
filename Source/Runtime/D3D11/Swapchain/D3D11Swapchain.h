#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Swapchain : public Swapchain
	{
	public:
		D3D11Swapchain(const SwapchainDesc& desc, D3D11Device* pDevice);
		~D3D11Swapchain() override = default;

		void PresentCore() override;
		void ResizeCore(Vector2u newSize) override;

		ComPtr<IDXGISwapChain>& GetD3DSwapchain() { return mD3DSwapchain; }
		ComPtr<ID3D11Texture2D> GetBackBuffer();
	private:
		ComPtr<IDXGISwapChain> mD3DSwapchain;
	};
}
