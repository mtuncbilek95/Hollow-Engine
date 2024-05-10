#include "CommandBuffer.h"

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>

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

	void CommandBuffer::BeginRenderPass(RenderPass** pRenderPasses, Vector4f colorPass)
	{
		BeginRenderPassImpl(pRenderPasses, colorPass);
		mBoundRenderPass = pRenderPasses[GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex()];
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassImpl();
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::BindPipeline(Pipeline** pPipelines)
	{
		BindPipelineImpl(pPipelines);
		mBoundPipeline = pPipelines[GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex()];
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

	void CommandBuffer::CopyBufferToTexture(GraphicsBuffer* pSourceBuffer, Texture* pDestinationTexture, BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureImpl(pSourceBuffer, pDestinationTexture, desc);
	}

	void CommandBuffer::SetTextureBarrier(Texture* pTexture, TextureBarrierUpdateDesc& desc)
	{
		SetTextureBarrierImpl(pTexture, desc);
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

	void CommandBuffer::BindDescriptors(DescriptorSet** ppSets, uint32 amount)
	{
		BindDescriptorsImpl(ppSets, amount);
	}

	void CommandBuffer::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
}