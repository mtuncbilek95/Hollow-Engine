#include "VulkanCommandBuffer.h"

#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Memory/VulkanMemoryUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>

#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>

namespace Hollow
{
	VulkanCommandBuffer::VulkanCommandBuffer(const CommandBufferDesc& desc, const SharedPtr<VulkanDevice> pDevice) : CommandBuffer(desc, pDevice),
		mVkCommandBuffer(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		auto pCommandPool = std::static_pointer_cast<VulkanCommandPool>(desc.pOwnerPool);
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pCommandPool->GetVkCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		CORE_ASSERT(vkAllocateCommandBuffers(pDevice->GetVkDevice(), &allocInfo, &mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to allocate command buffer");
	}

	void VulkanCommandBuffer::OnShutdown() noexcept
	{
		auto pCommandPool = std::static_pointer_cast<VulkanCommandPool>(GetOwnerPool());
		if (mVkCommandBuffer != VK_NULL_HANDLE)
			vkFreeCommandBuffers(mVkDevice, pCommandPool->GetVkCommandPool(), 1, &mVkCommandBuffer);

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

		CORE_ASSERT(vkBeginCommandBuffer(mVkCommandBuffer, &beginInfo) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to begin recording command buffer");
	}

	void VulkanCommandBuffer::EndRecordingImpl()
	{
		CORE_ASSERT(vkEndCommandBuffer(mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to end recording command buffer");
	}

	void VulkanCommandBuffer::BeginRenderPassImpl(SharedPtr<RenderPass> ppRenderPass[], Vector4f colorPass)
	{
		// Get the current image index
		uint32 imageIndex = GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex();

		auto pRenderPass = std::static_pointer_cast<VulkanRenderPass>(ppRenderPass[imageIndex]);

		// Get the render pass and framebuffer
		VkRenderPass renderPass = pRenderPass->GetVkRenderPass();
		VkFramebuffer framebuffer = pRenderPass->GetVkFramebuffer();

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { pRenderPass->GetTargetRenderSize().x, pRenderPass->GetTargetRenderSize().y };

		VkClearValue clearValues[2];
		clearValues[0].color = { colorPass.x, colorPass.y, colorPass.z, colorPass.w };

		if (pRenderPass->GetDepthStencilAttachment().pTextureBuffer != nullptr)
			clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(mVkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::EndRenderPassImpl()
	{
		vkCmdEndRenderPass(mVkCommandBuffer);
	}

	void VulkanCommandBuffer::BindPipelineImpl(SharedPtr<Pipeline> ppPipeline[])
	{
		uint32 imageIndex = GetOwnerDevice()->GetSwapchain()->GetCurrentFrameIndex();
		auto pPipeline = std::static_pointer_cast<VulkanPipeline>(ppPipeline[imageIndex]);
		vkCmdBindPipeline(mVkCommandBuffer, VulkanPipelineUtils::GetVkPipelineBindPoint(pPipeline->GetBindPoint()), pPipeline->GetVkPipeline());
	}

	void VulkanCommandBuffer::BindVertexBuffersImpl(SharedPtr<GraphicsBuffer> ppBuffer[], uint32 amount)
	{
		VkBuffer buffers[16];
		VkDeviceSize offsets[16];

		for (uint32 i = 0; i < amount; ++i)
		{
			auto pVulkanBuffer = std::static_pointer_cast<VulkanBuffer>(ppBuffer[i]);
			buffers[i] = pVulkanBuffer->GetVkBuffer();
			offsets[i] = 0;
		}

		vkCmdBindVertexBuffers(mVkCommandBuffer, 0, amount, buffers, offsets);
	}

	void VulkanCommandBuffer::BindIndexBufferImpl(SharedPtr<GraphicsBuffer> pBuffer, GraphicsIndexType indexType)
	{
		auto pVulkanBuffer = std::static_pointer_cast<VulkanBuffer>(pBuffer);
		vkCmdBindIndexBuffer(mVkCommandBuffer, pVulkanBuffer->GetVkBuffer(), 0, VulkanCoreUtils::GetVkIndexType(indexType));
	}

	void VulkanCommandBuffer::BindDescriptorsImpl(SharedPtr<DescriptorSet> ppSet[], uint32 amount)
	{
		auto pPipeline = std::static_pointer_cast<VulkanPipeline>(GetBoundPipeline());

		VkDescriptorSet sets[16];
		for (uint32 i = 0; i < amount; i++)
		{
			sets[i] = std::static_pointer_cast<VulkanDescriptorSet>(ppSet[i])->GetVkDescriptorSet();
		}

		vkCmdBindDescriptorSets(mVkCommandBuffer, VulkanPipelineUtils::GetVkPipelineBindPoint(pPipeline->GetBindPoint()), pPipeline->GetVkPipelineLayout(), 0, amount, sets, 0, nullptr);
	}

	void VulkanCommandBuffer::DrawIndexedImpl(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceOffset, uint32 instanceCount)
	{
		vkCmdDrawIndexed(mVkCommandBuffer, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}

	void VulkanCommandBuffer::SetViewportsImpl(ViewportDesc pViewports[], byte count)
	{
		VkViewport viewports[32];
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
	}

	void VulkanCommandBuffer::SetScissorsImpl(ScissorDesc pScissors[], byte count)
	{
		VkRect2D scissors[32];
		for (byte i = 0; i < count; i++)
		{
			scissors[i].offset = { static_cast<int>(pScissors[i].OffsetSize.x), static_cast<int>(pScissors[i].OffsetSize.y) };
			scissors[i].extent = { pScissors[i].ScissorSize.x, pScissors[i].ScissorSize.y };
		}

		vkCmdSetScissor(mVkCommandBuffer, 0, count, scissors);
	}

	void VulkanCommandBuffer::CopyBufferToBufferImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<GraphicsBuffer> pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		VkBuffer srcBuffer = std::static_pointer_cast<VulkanBuffer>(pSourceBuffer)->GetVkBuffer();
		VkBuffer dstBuffer = std::static_pointer_cast<VulkanBuffer>(pDestinationBuffer)->GetVkBuffer();

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = desc.SourceOffset;
		copyRegion.dstOffset = desc.DestinationOffset;
		copyRegion.size = desc.Size;

		vkCmdCopyBuffer(mVkCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	}

	void VulkanCommandBuffer::CopyBufferToTextureImpl(SharedPtr<GraphicsBuffer> pSourceBuffer, SharedPtr<Texture> pDestinationTexture, BufferTextureCopyDesc& desc)
	{
		VkBuffer srcBuffer = std::static_pointer_cast<VulkanBuffer>(pSourceBuffer)->GetVkBuffer();
		VkImage dstImage = std::static_pointer_cast<VulkanTexture>(pDestinationTexture)->GetVkTexture();

		VkBufferImageCopy copyRegion = {};
		copyRegion.bufferOffset = desc.BufferOffsetInBytes;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;

		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = desc.TargetMipIndex;
		copyRegion.imageSubresource.baseArrayLayer = desc.TargetArrayIndex;
		copyRegion.imageSubresource.layerCount = pDestinationTexture->GetArraySize();

		copyRegion.imageOffset = { desc.TextureOffset.x, desc.TextureOffset.y, desc.TextureOffset.z };
		copyRegion.imageExtent = { desc.TextureSize.x, desc.TextureSize.y, desc.TextureSize.z };

		vkCmdCopyBufferToImage(mVkCommandBuffer, srcBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
	}

	void VulkanCommandBuffer::SetTextureBarrierImpl(SharedPtr<Texture> pTexture, TextureBarrierUpdateDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetOwnerDevice());

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcAccessMask = VulkanMemoryUtils::GetVkAccessFlags(desc.SourceAccessMask);
		barrier.dstAccessMask = VulkanMemoryUtils::GetVkAccessFlags(desc.DestinationAccessMask);
		barrier.oldLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.OldLayout);
		barrier.newLayout = VulkanTextureUtils::GetVkTextureMemoryLayout(desc.NewLayout);
		barrier.srcQueueFamilyIndex = device->GetQueueFamilyIndex(desc.SourceQueue);
		barrier.dstQueueFamilyIndex = device->GetQueueFamilyIndex(desc.DestinationQueue);
		barrier.image = std::static_pointer_cast<VulkanTexture>(pTexture)->GetVkTexture();
		barrier.subresourceRange.aspectMask = VulkanTextureUtils::GetVkTextureAspectFlags(desc.AspectMask);
		barrier.subresourceRange.baseMipLevel = desc.MipIndex;
		barrier.subresourceRange.levelCount = pTexture->GetMipLevels();
		barrier.subresourceRange.baseArrayLayer = desc.ArrayIndex;
		barrier.subresourceRange.layerCount = pTexture->GetArraySize();
		barrier.pNext = nullptr;

		vkCmdPipelineBarrier(mVkCommandBuffer, VulkanPipelineUtils::GetVkPipelineStageFlags(desc.SourceStageFlags), VulkanPipelineUtils::GetVkPipelineStageFlags(desc.DestinationStageFlags), 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}
}
