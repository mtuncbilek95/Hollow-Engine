#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Device : public GraphicsDevice
	{
	public:
		D3D11Device(const GraphicsDeviceDesc& desc);
		~D3D11Device() override = default;

		const ComPtr<ID3D11Device>& GetD3DDevice() const { return mD3DDevice; }
		const ComPtr<ID3D11DeviceContext>& GetD3DContext() const { return mD3DContext; }

		SharedPtr<Swapchain>		CreateSwapchainCore(const SwapchainDesc& desc) override;
		SharedPtr<Shader>			CreateShaderCore(const ShaderDesc& desc) override;
		SharedPtr<GraphicsView>		CreateGraphicsViewCore(const GraphicsViewDesc& desc) override;
		SharedPtr<Texture>			CreateTextureCore(const TextureDesc& desc) override;
		SharedPtr<TextureView>		CreateTextureViewCore(const TextureViewDesc& desc) override;
		SharedPtr<Sampler>			CreateSamplerCore(const SamplerDesc& desc) override;
		SharedPtr<Pipeline>			CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) override;
		SharedPtr<Pipeline>			CreateComputePipelineCore(const ComputePipelineDesc& desc) override;
		SharedPtr<ResourceLayout>	CreateResourceLayoutCore(const ResourceLayoutDesc& desc) override;
		SharedPtr<CommandView>		CreateCommandViewCore(const CommandViewDesc& desc) override;
		SharedPtr<RenderPass>		CreateRenderPassCore(const RenderPassDesc& desc) override;

		void SubmitCommandViewCore(const Array<SharedPtr<CommandView>>& commandViews, const byte amount) override;

	private:
		ComPtr<ID3D11Device> mD3DDevice;
		ComPtr<ID3D11DeviceContext> mD3DContext;
	};
}
