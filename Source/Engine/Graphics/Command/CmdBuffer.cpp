#include "CmdBuffer.h"

#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	void CmdBuffer::BeginRecording()
	{
		BeginRecordingImpl();
		mRecording = true;
	}

	void CmdBuffer::EndRecording()
	{
		EndRecordingImpl();
		mRecording = false;
	}

	void CmdBuffer::BeginRendering(const DynamicPassDesc& desc)
	{
		BeginRenderingImpl(desc);
	}

	void CmdBuffer::EndRendering()
	{
		EndRenderingImpl();
	}

	void CmdBuffer::BindPipeline(WeakPtr<Pipeline> pPipeline)
	{
		BindPipelineImpl(pPipeline);
		mBoundPipeline = pPipeline;
	}

	void CmdBuffer::BindVertexBuffers(WeakPtr<GraphicsBuffer> ppBuffer[], u32 amount)
	{
		BindVertexBuffersImpl(ppBuffer, amount);
	}

	void CmdBuffer::BindIndexBuffer(WeakPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType)
	{
		BindIndexBufferImpl(pBuffer, indexType);
		mBoundIndexBuffer = pBuffer;
	}

	void CmdBuffer::BindDescriptors(WeakPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex)
	{
		BindDescriptorsImpl(ppSet, amount, setIndex);
	}

	void CmdBuffer::PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage)
	{
		PushConstantsImpl(buffer, offset, stage);
	}

	void CmdBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}

	void CmdBuffer::SetViewports(ViewportDesc pViewports[], u8 count)
	{
		SetViewportsImpl(pViewports, count);
	}

	void CmdBuffer::SetScissors(ScissorDesc pScissors[], u8 count)
	{
		SetScissorsImpl(pScissors, count);
	}

	void CmdBuffer::CopyBufferToBuffer(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferImpl(pSourceBuffer, pDestinationBuffer, desc);
	}

	void CmdBuffer::CopyBufferToTexture(WeakPtr<GraphicsBuffer> pSourceBuffer, WeakPtr<TextureImage> pDestinationTexture, BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureImpl(pSourceBuffer, pDestinationTexture, desc);
	}

	void CmdBuffer::SetTextureBarrier(WeakPtr<TextureImage> pTexture, TextureImageBarrier& desc)
	{
		SetTextureBarrierImpl(pTexture, desc);
	}

	void CmdBuffer::SetBufferBarrier(WeakPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrier& desc)
	{
		SetBufferBarrierImpl(pBuffer, desc);
	}


}