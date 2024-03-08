#include "VulkanCommandBuffer.h"

#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>

namespace Hollow
{
	VulkanCommandBuffer::VulkanCommandBuffer(const CommandBufferDesc& desc, VulkanDevice* pDevice) : CommandBuffer(desc, pDevice), mVkCommandBuffer(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = static_cast<VulkanCommandPool*>(desc.pOwnerPool)->GetVkCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		DEV_ASSERT(vkAllocateCommandBuffers(pDevice->GetVkDevice(), &allocInfo, &mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to allocate command buffer");
	}

	void VulkanCommandBuffer::OnShutdown() noexcept
	{
		if (mVkCommandBuffer != VK_NULL_HANDLE)
			vkFreeCommandBuffers(mVkDevice, static_cast<VulkanCommandPool*>(GetOwnerPool())->GetVkCommandPool(), 1, &mVkCommandBuffer);

		mVkCommandBuffer = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanCommandBuffer", "Command buffer has been destroyed");
	}

	void VulkanCommandBuffer::BeginRecordingImpl()
	{
		vkResetCommandBuffer(mVkCommandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		DEV_ASSERT(vkBeginCommandBuffer(mVkCommandBuffer, &beginInfo) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to begin recording command buffer");
	}

	void VulkanCommandBuffer::EndRecordingImpl()
	{
		DEV_ASSERT(vkEndCommandBuffer(mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to end recording command buffer");
	}

	void VulkanCommandBuffer::BeginRenderPassImpl(RenderPass* pRenderPass)
	{
		// Get the current image index
		uint32 imageIndex = GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex();

		// Get the render pass and framebuffer
		VkRenderPass renderPass = static_cast<VulkanRenderPass*>(pRenderPass)->GetVkRenderPass();
		VkFramebuffer framebuffer = static_cast<VulkanRenderPass*>(pRenderPass)->GetVkFramebuffer(imageIndex);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { pRenderPass->GetTargetRenderSize().x, pRenderPass->GetTargetRenderSize().y };

		VkClearValue clearValue;
		clearValue.color = { 0.1f, 0.2f, 0.3f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(mVkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::EndRenderPassImpl()
	{
		vkCmdEndRenderPass(mVkCommandBuffer);
	}

	void VulkanCommandBuffer::BindPipelineImpl(Pipeline* pPipeline)
	{
		VkPipeline pipeline = static_cast<VulkanPipeline*>(pPipeline)->GetVkPipeline();
		vkCmdBindPipeline(mVkCommandBuffer, VulkanPipelineUtils::GetVkPipelineBindPoint(pPipeline->GetBindPoint()), pipeline);
	}

	void VulkanCommandBuffer::BindVertexBuffersImpl(GraphicsBuffer** ppBuffer, uint32 amount)
	{
		VkBuffer buffers[16];
		VkDeviceSize offsets[16];

		for (uint32 i = 0; i < amount; i++)
		{
			buffers[i] = static_cast<VulkanBuffer*>(ppBuffer[i])->GetVkBuffer();
			offsets[i] = 0;
		}

		vkCmdBindVertexBuffers(mVkCommandBuffer, 0, amount, buffers, offsets);
	}

	void VulkanCommandBuffer::BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType)
	{
		VkBuffer buffer = static_cast<VulkanBuffer*>(pBuffer)->GetVkBuffer();
		vkCmdBindIndexBuffer(mVkCommandBuffer, buffer, 0, VulkanCommonUtils::GetVkIndexType(indexType));
	}

	void VulkanCommandBuffer::DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount)
	{
		vkCmdDrawIndexed(mVkCommandBuffer, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}

	void VulkanCommandBuffer::SetViewportsImpl(ViewportDesc* pViewports, byte count)
	{
		VkViewport* viewports = new VkViewport[count];
		for (byte i = 0; i < count; i++)
		{
			viewports[i].x = pViewports[i].OffsetSize.x;
			viewports[i].y = pViewports[i].OffsetSize.y;
			viewports[i].width = pViewports[i].ViewportSize.x;
			viewports[i].height = pViewports[i].ViewportSize.y;
			viewports[i].minDepth = pViewports[i].DepthRange.x;
			viewports[i].maxDepth = pViewports[i].DepthRange.y;
		}

		vkCmdSetViewport(mVkCommandBuffer, 0, count, viewports);

		delete[] viewports;
	}

	void VulkanCommandBuffer::SetScissorsImpl(ScissorDesc* pScissors, byte count)
	{
		VkRect2D* scissors = new VkRect2D[count];
		for (byte i = 0; i < count; i++)
		{
			scissors[i].offset = { static_cast<int>(pScissors[i].OffsetSize.x), static_cast<int>(pScissors[i].OffsetSize.y) };
			scissors[i].extent = { pScissors[i].ScissorSize.x, pScissors[i].ScissorSize.y };
		}

		vkCmdSetScissor(mVkCommandBuffer, 0, count, scissors);

		delete[] scissors;
	
	}

	void VulkanCommandBuffer::CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		VkBuffer srcBuffer = static_cast<VulkanBuffer*>(pSourceBuffer)->GetVkBuffer();
		VkBuffer dstBuffer = static_cast<VulkanBuffer*>(pDestinationBuffer)->GetVkBuffer();

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = desc.SourceOffset;
		copyRegion.dstOffset = desc.DestinationOffset;
		copyRegion.size = desc.Size;

		vkCmdCopyBuffer(mVkCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	}
}