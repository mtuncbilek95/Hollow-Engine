#include "CommandView.h"

namespace Hollow
{
	CommandView::CommandView(const CommandViewDesc& desc)
	{
	}

	void CommandView::BeginRecording()
	{
		mIsRecording = true;
		BeginRecordingCore();
	}

	void CommandView::EndRecording()
	{
		EndRecordingCore();
		mIsRecording = false;

		mBoundVertexBuffer.reset();
		mBoundIndexBuffer.reset();
		mBoundPipeline.reset();
	}

	void CommandView::BeginRenderPass(SharedPtr<RenderPass>& renderPass, const Vector4f& clearColor)
	{
		BeginRenderPassCore(renderPass, clearColor);
		mBoundRenderPass = renderPass;
	}

	void CommandView::EndRenderPass()
	{
		EndRenderPassCore();
		mBoundRenderPass.reset();
	}

	void CommandView::BindVertexBuffer(const SharedPtr<GraphicsView>& vertexBuffer)
	{
		BindVertexBufferCore(vertexBuffer);
		mBoundVertexBuffer = vertexBuffer;
	}

	void CommandView::UpdateBuffer(SharedPtr<GraphicsView> buffer, const GraphicsViewUpdateDesc& desc)
	{
		UpdateBufferCore(buffer, desc);
	}

	void CommandView::UpdateResourceLayout(SharedPtr<ResourceLayout> layout, const ResourceLayoutDesc& desc)
	{
		UpdateResourceLayoutCore(layout, desc);
	}

	void CommandView::BindIndexBuffer(const SharedPtr<GraphicsView>& indexBuffer, const IndexType type)
	{
		BindIndexBufferCore(indexBuffer, type);
		mBoundIndexBuffer = indexBuffer;
	}

	void CommandView::BindGraphicsPipeline(const SharedPtr<Pipeline>& pipeline)
	{
		BindGraphicsPipelineCore(pipeline);
		mBoundPipeline = pipeline;
	}

	void CommandView::BindComputePipeline(const SharedPtr<Pipeline>& pipeline)
	{
		BindComputePipelineCore(pipeline);
		mBoundPipeline = pipeline;
	}

	void CommandView::SetViewport(const ArrayList<ViewportDesc> viewports, const byte amount)
	{
		SetViewportCore(viewports, amount);
	}

	void CommandView::SetScissor(const ArrayList<ScissorDesc> scissors, const byte amount)
	{
		SetScissorCore(scissors, amount);
	}

	void CommandView::DrawIndexed(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset)
	{
		DrawIndexedCore(indexCount, indexOffset, vertexOffset);
	}

	void CommandView::CopyBufferToBuffer(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<GraphicsView>& dstBuffer, const BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferCore(srcBuffer, dstBuffer, desc);
	}

	void CommandView::CopyBufferToTexture(const SharedPtr<GraphicsView>& srcBuffer, const SharedPtr<Texture>& dstTexture, const BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureCore(srcBuffer, dstTexture, desc);
	}

	void CommandView::CopyTextureToTexture(const SharedPtr<Texture>& srcTexture, const SharedPtr<Texture>& dstTexture, const TextureTextureCopyDesc& desc)
	{
		CopyTextureToTextureCore(srcTexture, dstTexture, desc);
	}

	void CommandView::ClearTexture(const SharedPtr<Texture>& textureView, const Vector4f& clearColor)
	{
		ClearTextureCore(textureView, clearColor);
	}
}
