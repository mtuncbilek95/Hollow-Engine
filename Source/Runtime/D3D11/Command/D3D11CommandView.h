#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandView.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11CommandView : public CommandView
	{
	public:
		D3D11CommandView(const CommandViewDesc& desc, D3D11Device* pDevice);
		~D3D11CommandView() override = default;

		const ComPtr<ID3D11DeviceContext>& GetD3DDeferredContext() const { return mD3DDeferredContext; }
		const ComPtr<ID3D11CommandList>& GetD3DCommandList() const { return mD3DCommandList; }

		void ResetCommandList() { mD3DCommandList.Reset(); }

	protected:
		virtual void BeginRecordingCore() override;
		virtual void EndRecordingCore() override;
		virtual void BeginRenderPassCore(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor) override;
		virtual void EndRenderPassCore() override;
		virtual void BindVertexBufferCore(const SharedPtr<GraphicsView>& vertexBuffer) override;
		virtual void BindIndexBufferCore(const SharedPtr<GraphicsView>& indexBuffer, const IndexType type) override;
		virtual void BindGraphicsPipelineCore(const SharedPtr<Pipeline>& pipeline) override;
		virtual void BindComputePipelineCore(const SharedPtr<Pipeline>& pipeline) override;
		virtual void SetViewportCore(const ArrayList<ViewportDesc> viewports, const byte amount) override;
		virtual void SetScissorCore(const ArrayList<ScissorDesc> scissors, const byte amount) override;
		//virtual void CommitResourceCore() override;
		virtual void DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset) override;
		virtual void UpdateBufferCore(SharedPtr<GraphicsView> buffer, const GraphicsViewUpdateDesc& desc) override;
		virtual void UpdateResourceLayoutCore(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc) override;
		virtual void CopyBufferToBufferCore(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<GraphicsView>& dstBuffer, const BufferBufferCopyDesc& desc) override;
		virtual void CopyBufferToTextureCore(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc) override;
		virtual void CopyTextureToTextureCore(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc) override;
		virtual void ClearTextureCore(const SharedPtr<Texture>& textureView, const Vector4f& clearColor) override;

	private:
		ComPtr<ID3D11DeviceContext> mD3DDeferredContext;
		ComPtr<ID3D11CommandList> mD3DCommandList;
	};
}
