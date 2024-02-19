#include "CommandBuffer.h"

namespace Hollow
{
	CommandBuffer::CommandBuffer(const CommandBufferDesc& desc)
	{
	}

	void CommandBuffer::BeginRecording()
	{
		mIsRecording = true;
		BeginRecordingCore();
	}

	void CommandBuffer::EndRecording()
	{
		EndRecordingCore();
		mIsRecording = false;

		mBoundVertexBuffer.reset();
		mBoundIndexBuffer.reset();
		mBoundPipeline.reset();
	}

	void CommandBuffer::BeginRenderPass(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor)
	{
		BeginRenderPassCore(renderPass, clearColor);
		mBoundRenderPass = renderPass;
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassCore();
		mBoundRenderPass.reset();
	}

	void CommandBuffer::BindVertexBuffer(const SharedPtr<GraphicsBuffer>& vertexBuffer)
	{
		BindVertexBufferCore(vertexBuffer);
		mBoundVertexBuffer = vertexBuffer;
	}

	void CommandBuffer::BindIndexBuffer(const SharedPtr<GraphicsBuffer>& indexBuffer, const IndexType type)
	{
		BindIndexBufferCore(indexBuffer, type);
		mBoundIndexBuffer = indexBuffer;
	}

	void CommandBuffer::BindGraphicsPipeline(const SharedPtr<Pipeline>& pipeline)
	{
		BindGraphicsPipelineCore(pipeline);
		mBoundPipeline = pipeline;
	}

	void CommandBuffer::BindComputePipeline(const SharedPtr<Pipeline>& pipeline)
	{
		BindComputePipelineCore(pipeline);
		mBoundPipeline = pipeline;
	}

	void CommandBuffer::SetViewport(const Array<ViewportDesc> viewports, const byte amount)
	{
		SetViewportCore(viewports, amount);
	}

	void CommandBuffer::SetScissor(const Array<ScissorDesc> scissors, const byte amount)
	{
		SetScissorCore(scissors, amount);
	}

	void CommandBuffer::DrawIndexed(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset)
	{
		DrawIndexedCore(indexCount, indexOffset, vertexOffset);
	}

	void CommandBuffer::CopyBufferToBuffer(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<GraphicsBuffer>& dstBuffer, const BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferCore(srcBuffer, dstBuffer, desc);
	}

	void CommandBuffer::CopyBufferToTexture(const SharedPtr<GraphicsBuffer>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureCore(srcBuffer, dstTexture, desc);
	}

	void CommandBuffer::CopyTextureToTexture(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc)
	{
		CopyTextureToTextureCore(srcTexture, dstTexture, desc);
	}

	void CommandBuffer::ClearTexture(const SharedPtr<Texture>& textureView, const Vector4f& clearColor)
	{
		ClearTextureCore(textureView, clearColor);
	}
}
