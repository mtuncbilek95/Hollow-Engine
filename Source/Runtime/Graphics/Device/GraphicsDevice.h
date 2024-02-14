#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Device/GraphicsAPI.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

#include <Runtime/Graphics/Buffer/GraphicsBufferUpdateDesc.h>

namespace Hollow
{
	class GraphicsDeviceObject;
	class Swapchain;
	class Shader;
	class GraphicsBuffer;
	class Texture;
	class TextureView;
	class Sampler;
	class Pipeline;
	class ResourceLayout;

	struct SwapchainDesc;
	struct ShaderDesc;
	struct GraphicsBufferDesc;
	struct TextureDesc;
	struct TextureViewDesc;
	struct SamplerDesc;
	struct GraphicsPipelineDesc;
	struct ComputePipelineDesc;
	struct ResourceLayoutDesc;

	/**
	 * @class GraphicsDevice
	 * 
	 * @brief It is a both abstraction layer and usable interface via 
	 * higher abstractions. Responsible for creating and managing the
	 * objects that are related to the graphics device.
	 * 
	 */
	class RUNTIME_API GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() = default;

		SharedPtr<Swapchain>				CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Shader>					CreateShader(const ShaderDesc& desc);
		SharedPtr<GraphicsBuffer>			CreateGraphicsBuffer(const GraphicsBufferDesc& desc);
		SharedPtr<Texture>					CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureView>				CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Sampler>					CreateSampler(const SamplerDesc& desc);
		SharedPtr<Pipeline>					CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<Pipeline>					CreateComputePipeline(const ComputePipelineDesc& desc);
		SharedPtr<ResourceLayout>			CreateResourceLayout(const ResourceLayoutDesc& desc);

		// Mostly will be used for constant buffer updates
		void UpdateBuffer(SharedPtr<GraphicsBuffer> buffer, const GraphicsBufferUpdateDesc& desc);

		// Mostly will be used for updating resources that are need to be bound to related shader and pipeline.
		void UpdateResourceLayout(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc);

		//It will be used to destroy the device objects that are created by the graphics device.
		void DestroyDeviceObject(SharedPtr<GraphicsDeviceObject>& object);

		// Mostly will be used to call swapchain via GraphicsManager
		const SharedPtr<Swapchain>& GetSwapchain() const { return mSwapchain; }
		const SharedPtr<GraphicsAdapter>& GetGraphicsAdapter() const { return mGraphicsAdapter; }

		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

	protected:
		virtual SharedPtr<Swapchain>		CreateSwapchainCore(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<Shader>			CreateShaderCore(const ShaderDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer>	CreateGraphicsBufferCore(const GraphicsBufferDesc& desc) = 0;
		virtual SharedPtr<Texture>			CreateTextureCore(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureView>		CreateTextureViewCore(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Sampler>			CreateSamplerCore(const SamplerDesc& desc) = 0;
		virtual SharedPtr<Pipeline>			CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<Pipeline>			CreateComputePipelineCore(const ComputePipelineDesc& desc) = 0;
		virtual SharedPtr<ResourceLayout>	CreateResourceLayoutCore(const ResourceLayoutDesc& desc) = 0;

		virtual void UpdateBufferCore(SharedPtr<GraphicsBuffer> buffer, const GraphicsBufferUpdateDesc& desc) = 0;
		virtual void UpdateResourceLayoutCore(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc) = 0;

	protected:
		ArrayList<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;
		SharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;
		SharedPtr<GraphicsAdapter> mGraphicsAdapter;
	};
}
