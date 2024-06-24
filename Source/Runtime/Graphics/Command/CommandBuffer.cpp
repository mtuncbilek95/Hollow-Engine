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

	void CommandBuffer::BeginRendering(const DynamicPassDesc& desc)
	{
		BeginRenderingImpl(desc);
	}

	void CommandBuffer::EndRendering()
	{
		EndRenderingImpl();
	}

	void CommandBuffer::BeginRenderPass(SharedPtr<RenderPass> pRenderPass, SharedPtr<Framebuffer> pFramebuffer, const Vec4f& clearColor, const Vec2f& depthStencil)
	{
		BeginRenderPassImpl(pRenderPass, pFramebuffer, clearColor, depthStencil);
		mBoundRenderPass = pRenderPass;
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassImpl();
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::BindPipeline(SharedPtr<Pipeline> pPipeline)
	{
		BindPipelineImpl(pPipeline);
		mBoundPipeline = pPipeline;
	}

	void CommandBuffer::BindVertexBuffers(SharedPtr<GraphicsBuffer> ppBuffer[], u32 amount)
	{
		BindVertexBuffersImpl(ppBuffer, amount);
	}

	void CommandBuffer::BindIndexBuffer(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType)
	{
		BindIndexBufferImpl(pBuffer, indexType);
		mBoundIndexBuffer = pBuffer;
	}

	void CommandBuffer::BindDescriptors(SharedPtr<DescriptorSet> ppSet[], u32 amount, u32 setIndex)
	{
		BindDescriptorsImpl(ppSet, amount, setIndex);
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

	void CommandBuffer::ResolveTexture(SharedPtr<Texture> pSourceTexture, SharedPtr<Texture> pDestinationTexture)
	{
		ResolveTextureImpl(pSourceTexture, pDestinationTexture);
	}

	void CommandBuffer::SetTextureBarrier(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc)
	{
		SetTextureBarrierImpl(pTexture, desc);
	}

	void CommandBuffer::SetBufferMemoryBarrier(SharedPtr<GraphicsBuffer> pBuffer, BufferMemoryBarrierUpdateDesc& desc)
	{
		SetBufferMemoryBarrierImpl(pBuffer, desc);
	}

	void CommandBuffer::SetCommonMemoryBarrier(CommonMemoryBarrierUpdateDesc& desc)
	{
		SetCommonMemoryBarrierImpl(desc);
	}

	void CommandBuffer::PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage)
	{
		PushConstantsImpl(buffer, offset, stage);
	}

	void CommandBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
}