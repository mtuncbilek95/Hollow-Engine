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

		const ComPtr<ID3D11Device>& GetD3DDevice() { return mD3DDevice; }
		const ComPtr<ID3D11DeviceContext>& GetD3DContext() { return mD3DContext; }

		SharedPtr<Swapchain>		CreateSwapchainCore(const SwapchainDesc& desc) override;
		SharedPtr<Shader>			CreateShaderCore(const ShaderDesc& desc) override;
		SharedPtr<GraphicsBuffer>	CreateGraphicsBufferCore(const GraphicsBufferDesc& desc) override;
		SharedPtr<Texture>			CreateTextureCore(const TextureDesc& desc) override;
		SharedPtr<TextureView>		CreateTextureViewCore(const TextureViewDesc& desc) override;
		SharedPtr<Sampler>			CreateSamplerCore(const SamplerDesc& desc) override;
		SharedPtr<Pipeline>			CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) override;
		SharedPtr<Pipeline>			CreateComputePipelineCore(const ComputePipelineDesc& desc) override;
		SharedPtr<ResourceLayout>	CreateResourceLayoutCore(const ResourceLayoutDesc& desc) override;

		void UpdateBufferCore(SharedPtr<GraphicsBuffer> buffer, const GraphicsBufferUpdateDesc& desc) override;
		void UpdateResourceLayoutCore(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc) override;

	private:
		ComPtr<ID3D11Device> mD3DDevice;
		ComPtr<ID3D11DeviceContext> mD3DContext;
	};
}
