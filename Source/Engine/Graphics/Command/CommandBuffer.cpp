#include "CommandBuffer.h"

#include <Engine/Graphics/Device/GraphicsDevice.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>

namespace Hollow
{
	void CommandBuffer::BeginRecording()
	{
		BeginRecordingImpl();
		mRecording = true;
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

	void CommandBuffer::PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage)
	{
		PushConstantsImpl(buffer, offset, stage);
	}

	void CommandBuffer::SetViewports(ViewportDesc pViewports[], u8 count)
	{
		SetViewportsImpl(pViewports, count);
	}

	void CommandBuffer::SetScissors(ScissorDesc pScissors[], u8 count)
	{
		SetScissorsImpl(pScissors, count);
	}

	void CommandBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount)
	{
		DrawIndexedImpl(indexCount, indexOffset, vertexOffset, instanceOffset, instanceCount);
	}
}