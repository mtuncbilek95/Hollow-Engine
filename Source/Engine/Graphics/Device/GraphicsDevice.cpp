#include "GraphicsDevice.h"

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(SharedInstance pInstance) : mInstance(pInstance)
	{
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
	{
		return CreateQueueImpl(desc);
	}

	SharedPtr<GraphicsMemory> GraphicsDevice::CreateMemory(const GraphicsMemoryDesc& desc)
	{
		return CreateMemoryImpl(desc);
	}

	SharedPtr<TextureImage> GraphicsDevice::CreateTextureImage(const TextureImageDesc& desc)
	{
		return CreateTextureImageImpl(desc);
	}

	SharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		return CreateTextureViewImpl(desc);
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		return CreateSamplerImpl(desc);
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateBuffer(const GraphicsBufferDesc& desc)
	{
		return CreateBufferImpl(desc);
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		return CreateShaderImpl(desc);
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		return CreateSwapchainImpl(desc);
	}
}
