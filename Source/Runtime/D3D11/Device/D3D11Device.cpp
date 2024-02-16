#include "D3D11Device.h"

#include <Runtime/D3D11/Adapter/D3D11Adapter.h>
#include <Runtime/D3D11/Swapchain/D3D11Swapchain.h>
#include <Runtime/D3D11/Shader/D3D11Shader.h>
#include <Runtime/D3D11/Buffer/D3D11GraphicsView.h>
#include <Runtime/D3D11/Texture/D3D11TextureView.h>
#include <Runtime/D3D11/Texture/D3D11Texture.h>
#include <Runtime/D3D11/Command/D3D11CommandView.h>
#include <Runtime/D3D11/Pipeline/D3D11Pipeline.h>
//#include <Runtime/D3D11/ResourceLayout/D3D11ResourceLayout.h>
#include <Runtime/D3D11/Command/D3D11CommandView.h>

namespace Hollow
{
	D3D11Device::D3D11Device(const GraphicsDeviceDesc& desc) : GraphicsDevice(desc)
	{
		IDXGIAdapter1* adapter = static_cast<D3D11Adapter*>(desc.Adapter.get())->GetD3DAdapter().Get();

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

#ifdef HOLLOW_DEBUG
		UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
		UINT flags = 0;
#endif
		
		DEV_ASSERT(SUCCEEDED(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN,
			nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &mD3DDevice, &featureLevel, &mD3DContext)), "D3D11Device", "Failed to create D3D11Device");

		CORE_LOG(HE_VERBOSE, "D3D11Device", "Operation is successful.");
	}

	SharedPtr<Swapchain> D3D11Device::CreateSwapchainCore(const SwapchainDesc& desc)
	{
		return std::make_shared<D3D11Swapchain>(desc, mD3DDevice.Get());
	}

	SharedPtr<Shader> D3D11Device::CreateShaderCore(const ShaderDesc& desc)
	{
		return std::make_shared<D3D11Shader>(desc, mD3DDevice.Get());
	}

	SharedPtr<GraphicsView> D3D11Device::CreateGraphicsViewCore(const GraphicsViewDesc& desc)
	{
		return SharedPtr<GraphicsView>();
	}

	SharedPtr<Texture> D3D11Device::CreateTextureCore(const TextureDesc& desc)
	{
		return std::make_shared<D3D11Texture>(desc, mD3DDevice.Get());
	}

	SharedPtr<TextureView> D3D11Device::CreateTextureViewCore(const TextureViewDesc& desc)
	{
		return std::make_shared<D3D11TextureView>(desc, mD3DDevice.Get());
	}

	SharedPtr<Sampler> D3D11Device::CreateSamplerCore(const SamplerDesc& desc)
	{
		return SharedPtr<Sampler>();
	}

	SharedPtr<Pipeline> D3D11Device::CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc)
	{
		return SharedPtr<Pipeline>();
	}

	SharedPtr<Pipeline> D3D11Device::CreateComputePipelineCore(const ComputePipelineDesc& desc)
	{
		return SharedPtr<Pipeline>();
	}

	SharedPtr<ResourceLayout> D3D11Device::CreateResourceLayoutCore(const ResourceLayoutDesc& desc)
	{
		return SharedPtr<ResourceLayout>();
	}

	SharedPtr<CommandView> D3D11Device::CreateCommandViewCore(const CommandViewDesc& desc)
	{
		return SharedPtr<CommandView>();
	}

	void D3D11Device::SubmitCommandViewCore(const ArrayList<SharedPtr<CommandView>>& commandViews, const byte amount)
	{
		for (auto& commandView : commandViews)
		{
			mD3DContext->ExecuteCommandList(std::dynamic_pointer_cast<D3D11CommandView>(commandView)->GetD3DCommandList().Get(), false);
			std::dynamic_pointer_cast<D3D11CommandView>(commandView)->ResetCommandList();
		}
	}
}
