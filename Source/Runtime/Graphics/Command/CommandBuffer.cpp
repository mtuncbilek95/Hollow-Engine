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

	void CommandBuffer::BeginRenderPass(SharedPtr<RenderPass> ppRenderPass[], Vector4f colorPass)
	{
		BeginRenderPassImpl(ppRenderPass, colorPass);
		mBoundRenderPass = ppRenderPass[GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex()];
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassImpl();
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::BindPipeline(SharedPtr<Pipeline> ppPipeline[])
	{
		BindPipelineImpl(ppPipeline);
		mBoundPipeline = ppPipeline[GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex()];
	}

	void CommandBuffer::BindVertexBuffers(SharedPtr<GraphicsBuffer> ppBuffer[], uint32 amount)
	{
		BindVertexBuffersImpl(ppBuffer, amount);
	}

	void CommandBuffer::BindIndexBuffer(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType)
	{
		BindIndexBufferImpl(pBuffer, indexType);
		mBoundIndexBuffer = pBuffer;
	}

	void CommandBuffer::BindDescriptors(SharedPtr<DescriptorSet> ppSet[], uint32 amount)
	{
		BindDescriptorsImpl(ppSet, amount);
	}

	void CommandBuffer::SetViewports(ViewportDesc pViewports[], byte count)
	{
		SetViewportsImpl(pViewports, count);
	}

	void CommandBuffer::SetScissors(ScissorDesc pScissors[], byte count)
	{
		SetScissorsImpl(pScissors, count);
	}

	void CommandBuffer::CopyBufferToBuffer(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferImpl(pSourceBuffer, pDestinationBuffer, desc);
	}

	void CommandBuffer::CopyBufferToTexture(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureImpl(pSourceBuffer, pDestinationTexture, desc);
	}

	void CommandBuffer::SetTextureBarrier(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc)
	{
		SetTextureBarrierImpl(pTexture, desc);
	}

	void CommandBuffer::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
}