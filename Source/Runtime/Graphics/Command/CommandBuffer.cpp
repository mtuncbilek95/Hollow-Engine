#include "CommandBuffer.h"

namespace Hollow
{
	void CommandBuffer::BeginRecording()
	{
		mRecording = true;
		BeginRecordingImpl();
	}

	void CommandBuffer::EndRecording()
	{
		EndRecordingImpl();
		mRecording = false;
	}

	void CommandBuffer::BeginRenderPass(RenderPass* ppRenderPass)
	{
		BeginRenderPassImpl(ppRenderPass);
		mBoundRenderPass = ppRenderPass;
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassImpl();
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::BindPipeline(Pipeline* pPipeline)
	{
		BindPipelineImpl(pPipeline);
		mBoundPipeline = pPipeline;
	}

	void CommandBuffer::SetViewports(ViewportDesc* pViewports, byte count)
	{
		SetViewportsImpl(pViewports, count);
	}

	void CommandBuffer::SetScissors(ScissorDesc* pScissors, byte count)
	{
		SetScissorsImpl(pScissors, count);
	}

	void CommandBuffer::CopyBufferToBuffer(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferImpl(pSourceBuffer, pDestinationBuffer, desc);
	}

	void CommandBuffer::BindVertexBuffers(GraphicsBuffer** ppBuffer, uint32 amount)
	{
		BindVertexBuffersImpl(ppBuffer, amount);
	}

	void CommandBuffer::BindIndexBuffer(GraphicsBuffer* pBuffer, GraphicsIndexType indexType)
	{
		BindIndexBufferImpl(pBuffer, indexType);
		mBoundIndexBuffer = pBuffer;
	}

	void CommandBuffer::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
}