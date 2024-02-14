#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Swapchain : public Swapchain
	{
	public:
		D3D11Swapchain(const SwapchainDesc& desc, ID3D11Device* pDevice);
		~D3D11Swapchain() override = default;

		void PresentCore() override;
		void ResizeCore(Vector2u newSize) override;

		ComPtr<IDXGISwapChain>& GetSwapchain() { return mD3DSwapchain; }

	private:
		ID3D11Device* mD3DDevice;
		ComPtr<IDXGISwapChain> mD3DSwapchain;
	};
}
