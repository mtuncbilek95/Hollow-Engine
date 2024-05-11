#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Core/GraphicsIndexType.h>
#include <Runtime/Graphics/Core/ViewportDesc.h>
#include <Runtime/Graphics/Core/ScissorDesc.h>
#include <Runtime/Graphics/Core/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Core/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Core/TextureBarrierUpdateDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsBuffer;
	class RUNTIME_API Pipeline;
	class RUNTIME_API RenderPass;
	class RUNTIME_API DescriptorSet;
	class RUNTIME_API Texture;


	/**
	 * @class CommandBuffer
	 * 
	 * @brief Represents a command buffer that can be recorded and submitted to the GPU.
	 */
	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mBoundIndexBuffer(nullptr),
		mBoundPipeline(nullptr), mBoundRenderPass(nullptr), mRecording(false) 
		{}
		virtual ~CommandBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRenderPass(SharedPtr<RenderPass> ppRenderPass[], Vector4f colorPass);
		void EndRenderPass();
		void BindPipeline(SharedPtr<Pipeline> ppPipeline[]);
		void BindVertexBuffers(SharedPtr<GraphicsBuffer> pBuffer[], uint32 amount);
		void BindIndexBuffer(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(SharedPtr<DescriptorSet> ppSet[], uint32 amount);
		void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount);
		void SetViewports(ViewportDesc pViewports[], byte count);
		void SetScissors(ScissorDesc pScissors[], byte count);
		void CopyBufferToBuffer(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc);
		void SetTextureBarrier(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc);

		SharedPtr<CommandPool> GetOwnerPool() const noexcept { return mOwnerPool; }
		SharedPtr<GraphicsBuffer> GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		SharedPtr<Pipeline> GetBoundPipeline() const noexcept { return mBoundPipeline; }
		SharedPtr<RenderPass> GetBoundRenderPass() const noexcept { return mBoundRenderPass; }
		bool IsRecording() const noexcept { return mRecording; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::CommandBuffer; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderPassImpl(SharedPtr<RenderPass> ppRenderPass[], Vector4f colorPass) = 0;
		virtual void EndRenderPassImpl() = 0;
		virtual void BindPipelineImpl(SharedPtr<Pipeline> ppPipeline[]) = 0;
		virtual void BindVertexBuffersImpl(SharedPtr<GraphicsBuffer> ppBuffer[], uint32 amount) = 0;
		virtual void BindIndexBufferImpl(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(SharedPtr<DescriptorSet> ppSet[], uint32 amount) = 0;
		virtual void DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], byte count) = 0;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], byte count) = 0;
		virtual void CopyBufferToBufferImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc) = 0;
		virtual void SetTextureBarrierImpl(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc) = 0;
	private:
		SharedPtr<CommandPool> mOwnerPool;
		SharedPtr<GraphicsBuffer> mBoundIndexBuffer;
		SharedPtr<Pipeline> mBoundPipeline;
		SharedPtr<RenderPass> mBoundRenderPass;
		bool mRecording;
	};
}