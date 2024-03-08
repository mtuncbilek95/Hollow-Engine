#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Common/GraphicsIndexType.h>
#include <Runtime/Graphics/Common/ViewportDesc.h>
#include <Runtime/Graphics/Common/ScissorDesc.h>
#include <Runtime/Graphics/Command/BufferBufferCopyDesc.h>

namespace Hollow
{
	class GraphicsBuffer;
	class Pipeline;
	class RenderPass;

	class RUNTIME_API CommandBuffer : public GraphicsDeviceObject
	{
	public:
		CommandBuffer(const CommandBufferDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool) {}
		virtual ~CommandBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRenderPass(RenderPass* ppRenderPass);
		void EndRenderPass();
		void BindPipeline(Pipeline* pPipeline);
		void BindVertexBuffers(GraphicsBuffer** ppBuffer, uint32 amount);
		void BindIndexBuffer(GraphicsBuffer* pBuffer, GraphicsIndexType indexType);
		void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount);
		void SetViewports(ViewportDesc* pViewports, byte count);
		void SetScissors(ScissorDesc* pScissors, byte count);
		void CopyBufferToBuffer(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc);

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
		virtual void BeginRenderPassImpl(RenderPass* ppRenderPass) = 0;
		virtual void EndRenderPassImpl() = 0;
		virtual void BindPipelineImpl(Pipeline* pPipeline) = 0;
		virtual void BindVertexBuffersImpl(GraphicsBuffer** ppBuffer, uint32 amount) = 0;
		virtual void BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType) = 0;
		virtual void DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc* pViewports, byte count) = 0;
		virtual void SetScissorsImpl(ScissorDesc* pScissors, byte count) = 0;
		virtual void CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;

	private:
		CommandPool* mOwnerPool;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		RenderPass* mBoundRenderPass;
		bool mRecording;
	};
}