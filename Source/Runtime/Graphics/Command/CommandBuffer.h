#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Common/GraphicsIndexType.h>
#include <Runtime/Graphics/Common/ViewportDesc.h>
#include <Runtime/Graphics/Common/ScissorDesc.h>
#include <Runtime/Graphics/Command/BufferBufferCopyDesc.h>
#include <Runtime/Graphics/Command/BufferTextureCopyDesc.h>
#include <Runtime/Graphics/Command/TextureBarrierUpdateDesc.h>

namespace Hollow
{
	class GraphicsBuffer;
	class Pipeline;
	class RenderPass;
	class DescriptorSet;
	class Texture;


	/**
	 * @class CommandBuffer
	 * 
	 * @brief Represents a command buffer that can be recorded and submitted to the GPU.
	 */
	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mBoundIndexBuffer(nullptr),
		mBoundPipeline(nullptr), mBoundRenderPass(nullptr), mRecording(false) 
		{}
		virtual ~CommandBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRenderPass(RenderPass** pRenderPasses, Vector4f colorPass);
		void EndRenderPass();
		void BindPipeline(Pipeline** pPipelines);
		void BindVertexBuffers(GraphicsBuffer** ppBuffer, uint32 amount);
		void BindIndexBuffer(GraphicsBuffer* pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(DescriptorSet** ppSets, uint32 amount);
		void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount);
		void SetViewports(ViewportDesc* pViewports, byte count);
		void SetScissors(ScissorDesc* pScissors, byte count);
		void CopyBufferToBuffer(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(GraphicsBuffer* pSourceBuffer, Texture* pDestinationTexture, BufferTextureCopyDesc& desc);
		void SetTextureBarrier(Texture* pTexture, TextureBarrierUpdateDesc& desc);

		CommandPool* GetOwnerPool() const noexcept { return mOwnerPool; }
		GraphicsBuffer* GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		Pipeline* GetBoundPipeline() const noexcept { return mBoundPipeline; }
		RenderPass* GetBoundRenderPass() const noexcept { return mBoundRenderPass; }
		bool IsRecording() const noexcept { return mRecording; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::CommandBuffer; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderPassImpl(RenderPass** pRenderPasses, Vector4f colorPass) = 0;
		virtual void EndRenderPassImpl() = 0;
		virtual void BindPipelineImpl(Pipeline** pPipelines) = 0;
		virtual void BindVertexBuffersImpl(GraphicsBuffer** ppBuffer, uint32 amount) = 0;
		virtual void BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(DescriptorSet** ppSets, uint32 amount) = 0;
		virtual void DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc* pViewports, byte count) = 0;
		virtual void SetScissorsImpl(ScissorDesc* pScissors, byte count) = 0;
		virtual void CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureImpl(GraphicsBuffer* pSourceBuffer, Texture* pDestinationTexture, BufferTextureCopyDesc& desc) = 0;
		virtual void SetTextureBarrierImpl(Texture* pTexture, TextureBarrierUpdateDesc& desc) = 0;
	private:
		CommandPool* mOwnerPool;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		RenderPass* mBoundRenderPass;
		bool mRecording;
	};
}