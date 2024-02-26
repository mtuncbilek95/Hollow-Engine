#include "CommandBuffer.h"

namespace Hollow
{
	CommandBuffer::CommandBuffer(const CommandBufferDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), 
		mBoundIndexBuffer(nullptr), mBoundPipeline(nullptr), mBoundRenderPass(nullptr), mIsRecording(false)
	{
		mIsRecording = false;
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

		mBoundIndexBuffer = nullptr;
		mBoundPipeline = nullptr;
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::BindVertexBuffers(GraphicsBuffer** vertexBuffer, byte amount)
	{
		BindVertexBuffersCore(vertexBuffer, amount);
	}

	void CommandBuffer::BindIndexBuffer(GraphicsBuffer* indexBuffer, const IndexSizeType type)
	{
		BindIndexBufferCore(indexBuffer, type);

		mBoundIndexBuffer = indexBuffer;
	}

	void CommandBuffer::DrawIndexed(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset)
	{
		DrawIndexedCore(indexCount, indexOffset, vertexOffset);
	}

	void CommandBuffer::DispatchCompute(const uint32 groupCountX, const uint32 groupCountY, const uint32 groupCountZ)
	{
		DispatchComputeCore(groupCountX, groupCountY, groupCountZ);
	}

	void CommandBuffer::BindPipeline(Pipeline* pipeline)
	{
		BindPipelineCore(pipeline);

		mBoundPipeline = pipeline;
	}

	void CommandBuffer::BeginRenderPass(RenderPass* renderPass, const Vector4f& clearColor, const byte ColorValueCount, const double clearDepth, const double clearStencil)
	{
		BeginRenderPassCore(renderPass, clearColor, ColorValueCount, clearDepth, clearStencil);

		mBoundRenderPass = renderPass;
	}

	void CommandBuffer::EndRenderPass()
	{
		EndRenderPassCore();

		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::SetViewports(ViewportDesc* viewports, const byte amount)
	{
		SetViewportsCore(viewports, amount);
	}

	void CommandBuffer::SetScissors(ScissorDesc* scissors, const byte amount)
	{
		SetScissorsCore(scissors, amount);
	}

	void CommandBuffer::ClearBounds()
	{
		mBoundIndexBuffer = nullptr;
		mBoundPipeline = nullptr;
		mBoundRenderPass = nullptr;
	}

	void CommandBuffer::CopyBufferToBuffer(const GraphicsBuffer* srcBuffer, const GraphicsBuffer* dstBuffer, const BufferBufferCopyDesc& desc)
	{
		CopyBufferToBufferCore(srcBuffer, dstBuffer, desc);
	}

	void CommandBuffer::CopyBufferToTexture(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc)
	{
		CopyBufferToTextureCore(srcBuffer, dstTexture, desc);
	}

	void CommandBuffer::CopyTextureToTexture(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc)
	{
		CopyTextureToTextureCore(srcTexture, dstTexture, desc);
	}

	void CommandBuffer::SetTextureMemoryBarrier(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
		SetTextureMemoryBarrierCore(pTexture, desc);
	}

	void CommandBuffer::SetBufferMemoryBarrier(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
		SetBufferMemoryBarrierCore(pBuffer, desc);
	}

	void CommandBuffer::CommitResourceSets(DescriptorSet** ppSets, const byte amount)
	{
		CommitResourceSetsCore(ppSets, amount);
	}
}
