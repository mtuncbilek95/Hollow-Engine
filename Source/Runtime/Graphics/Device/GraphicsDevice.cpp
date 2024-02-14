#include "GraphicsDevice.h"

#include <Runtime/Graphics/Manager/GraphicsManager.h>

#include <Runtime/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : mGraphicsAdapter(desc.Adapter), mGraphicsAPI(desc.API)
	{
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		if (mSwapchain != nullptr)
		{
			return mSwapchain;
		}

		SharedPtr<Swapchain> swapchain = CreateSwapchainCore(desc);
		mSwapchain = swapchain;
		mDeviceObjects.push_back(swapchain);
		return swapchain;
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		SharedPtr<Shader> shader = CreateShaderCore(desc);

		return nullptr;
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateGraphicsBuffer(const GraphicsBufferDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateComputePipeline(const ComputePipelineDesc& desc)
	{
		return nullptr;
	}

	SharedPtr<ResourceLayout> GraphicsDevice::CreateResourceLayout(const ResourceLayoutDesc& desc)
	{
		return nullptr;
	}

	void GraphicsDevice::UpdateBuffer(SharedPtr<GraphicsBuffer> buffer, const GraphicsBufferUpdateDesc& desc)
	{
	}

	void GraphicsDevice::UpdateResourceLayout(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc)
	{
	}

	void GraphicsDevice::DestroyDeviceObject(SharedPtr<GraphicsDeviceObject>& object)
	{
	}
}
