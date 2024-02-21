#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Command/ViewportDesc.h>
#include <Runtime/Graphics/Command/ScissorDesc.h>
#include <Runtime/Graphics/Command/IndexType.h>
#include <Runtime/Graphics/Command/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Command/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Command/TextureTextureCopyDesc.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUpdateDesc.h>

namespace Hollow
{
	class GraphicsBuffer;
	class Pipeline;
	class Texture;
	class ResourceLayout;
	class RenderPass;

	struct ResourceLayoutDesc;

	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, GraphicsDevice* pDevice);
		virtual ~CommandBuffer() override = default;

		const SharedPtr<GraphicsBuffer>& GetBoundVertexBuffer() const { return mBoundVertexBuffer; }
		const SharedPtr<GraphicsBuffer>& GetBoundIndexBuffer() const { return mBoundIndexBuffer; }
		const SharedPtr<Pipeline>& GetBoundPipeline() const { return mBoundPipeline; }
		bool IsRecording() const { return mIsRecording; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::CommandBuffer; }

		void BeginRecording();
		void EndRecording();
		void BeginRenderPass(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor);
		void EndRenderPass();
		void BindVertexBuffer(const SharedPtr<GraphicsBuffer>& vertexBuffer);
		void BindIndexBuffer(const SharedPtr<GraphicsBuffer>& indexBuffer, const IndexType type);
		void BindGraphicsPipeline(const SharedPtr<Pipeline>& pipeline);
		void BindComputePipeline(const SharedPtr<Pipeline>& pipeline);
		void SetViewport(const Array<ViewportDesc> viewports, const byte amount);
		void SetScissor(const Array<ScissorDesc> scissors, const byte amount);
		void DrawIndexed(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset);
		void CopyBufferToBuffer(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<GraphicsBuffer>& dstBuffer, const BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc);
		void CopyTextureToTexture(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc);
		void ClearTexture(const SharedPtr<Texture>& textureView, const Vector4f& clearColor);

	protected:
		virtual void BeginRecordingCore() = 0;
		virtual void EndRecordingCore() = 0;
		virtual void BeginRenderPassCore(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor) = 0;
		virtual void EndRenderPassCore() = 0;
		virtual void BindVertexBufferCore(const SharedPtr<GraphicsBuffer>& vertexBuffer) = 0;
		virtual void BindIndexBufferCore(const SharedPtr<GraphicsBuffer>& indexBuffer, const IndexType type) = 0;
		virtual void BindGraphicsPipelineCore(const SharedPtr<Pipeline>& pipeline) = 0;
		virtual void BindComputePipelineCore(const SharedPtr<Pipeline>& pipeline) = 0;
		virtual void SetViewportCore(const Array<ViewportDesc> viewports, const byte amount) = 0;
		virtual void SetScissorCore(const Array<ScissorDesc> scissors, const byte amount) = 0;
		virtual void DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset) = 0;
		virtual void UpdateBufferCore(SharedPtr<GraphicsBuffer> buffer, const GraphicsBufferUpdateDesc& desc) = 0;
		virtual void UpdateResourceLayoutCore(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc) = 0;
		virtual void CopyBufferToBufferCore(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<GraphicsBuffer>& dstBuffer, const BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureCore(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc) = 0;
		virtual void CopyTextureToTextureCore(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc) = 0;
		virtual void ClearTextureCore(const SharedPtr<Texture>& textureView, const Vector4f& clearColor) = 0;

	private:
		SharedPtr<GraphicsBuffer> mBoundVertexBuffer;
		SharedPtr<GraphicsBuffer> mBoundIndexBuffer;
		SharedPtr<Pipeline> mBoundPipeline;
		SharedPtr<RenderPass> mBoundRenderPass;
		bool mIsRecording;
	};
}
