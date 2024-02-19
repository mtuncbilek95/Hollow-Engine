#include "GraphicsDevice.h"

#include <Runtime/Graphics/Manager/GraphicsManager.h>

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Texture/Texture.h>	
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>



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
		swapchain->_SetOwnerDevice(this);
		mSwapchain = swapchain;
		mDeviceObjects.push_back(swapchain);
		return swapchain;
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		SharedPtr<Shader> shader = CreateShaderCore(desc);
		shader->_SetOwnerDevice(this);
		mDeviceObjects.push_back(shader);
		return shader;
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateGraphicsBuffer(const GraphicsBufferDesc& desc)
	{
		SharedPtr<GraphicsBuffer> graphicsBuffer = CreateGraphicsBufferCore(desc);
		graphicsBuffer->_SetOwnerDevice(this);
		mDeviceObjects.push_back(graphicsBuffer);
		return graphicsBuffer;
	}

	SharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		SharedPtr<Texture> texture = CreateTextureCore(desc);
		texture->_SetOwnerDevice(this);
		mDeviceObjects.push_back(texture);
		return texture;
	}

	SharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		SharedPtr<TextureView> textureView = CreateTextureViewCore(desc);
		textureView->_SetOwnerDevice(this);
		mDeviceObjects.push_back(textureView);
		return textureView;
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		SharedPtr<Sampler> sampler = CreateSamplerCore(desc);
		sampler->_SetOwnerDevice(this);
		mDeviceObjects.push_back(sampler);
		return sampler;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		SharedPtr<Pipeline> pipeline = CreateGraphicsPipelineCore(desc);
		pipeline->_SetOwnerDevice(this);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateComputePipeline(const ComputePipelineDesc& desc)
	{
		SharedPtr<Pipeline> pipeline = CreateComputePipelineCore(desc);
		pipeline->_SetOwnerDevice(this);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	SharedPtr<CommandBuffer> GraphicsDevice::CreateCommandBuffer(const CommandBufferDesc& desc)
	{
		SharedPtr<CommandBuffer> commandView = CreateCommandBufferCore(desc);
		commandView->_SetOwnerDevice(this);
		mDeviceObjects.push_back(commandView);
		return commandView;
	}

	SharedPtr<RenderPass> GraphicsDevice::CreateRenderPass(const RenderPassDesc& desc)
	{
		SharedPtr<RenderPass> renderPass = CreateRenderPassCore(desc);
		renderPass->_SetOwnerDevice(this);
		mDeviceObjects.push_back(renderPass);
		return renderPass;
	}
}
