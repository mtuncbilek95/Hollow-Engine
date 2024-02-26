#include "VulkanCommandBuffer.h"

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Buffer/VulkanGraphicsBuffer.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>

namespace Hollow
{
	VulkanCommandBuffer::VulkanCommandBuffer(const CommandBufferDesc& desc, VulkanDevice* pDevice) : CommandBuffer(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		mVkCommandPool = static_cast<VulkanCommandPool*>(desc.pCommandPool)->GetVkCommandPool();

		// Allocate command buffer
		VkCommandBufferAllocateInfo allocInfo = {};

		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mVkCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		allocInfo.pNext = nullptr;

		DEV_ASSERT(vkAllocateCommandBuffers(mVkLogicalDevice, &allocInfo, &mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to allocate command buffer");

	}

	void VulkanCommandBuffer::OnShutdown() noexcept
	{
		if (mVkCommandBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(mVkLogicalDevice, mVkCommandPool, 1, &mVkCommandBuffer);
			mVkCommandBuffer = VK_NULL_HANDLE;
		}
	}

	void VulkanCommandBuffer::BeginRecordingCore()
	{
		// Reset command buffer
		DEV_ASSERT(vkResetCommandBuffer(mVkCommandBuffer, VkCommandBufferResetFlags()) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to reset command buffer");

		// Begin recording
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;
		beginInfo.pNext = nullptr;

		DEV_ASSERT(vkBeginCommandBuffer(mVkCommandBuffer, &beginInfo) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to begin recording command buffer");
	}

	void VulkanCommandBuffer::EndRecordingCore()
	{
		DEV_ASSERT(vkEndCommandBuffer(mVkCommandBuffer) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to end recording command buffer");
	}

	void VulkanCommandBuffer::BindVertexBuffersCore(GraphicsBuffer** vertexBuffer, byte amount)
	{
		VkBuffer* pBuffers = new VkBuffer[amount];
		VkDeviceSize* pOffsets = new VkDeviceSize[amount];

		uint64 offset = 0;
		for (byte i = 0; i < amount; i++)
		{
			VulkanGraphicsBuffer* pBuffer = static_cast<VulkanGraphicsBuffer*>(vertexBuffer[i]);
			pBuffers[i] = pBuffer->GetVkBuffer();
			pOffsets[i] = offset;
			offset += pBuffer->GetSizeInBytes();
		}

		vkCmdBindVertexBuffers(mVkCommandBuffer, 0, amount, pBuffers, pOffsets);

		delete[] pBuffers;
		delete[] pOffsets;
	}

	void VulkanCommandBuffer::BindIndexBufferCore(GraphicsBuffer* indexBuffer, const IndexSizeType type)
	{
		VulkanGraphicsBuffer* pBuffer = static_cast<VulkanGraphicsBuffer*>(indexBuffer);

		vkCmdBindIndexBuffer(mVkCommandBuffer, pBuffer->GetVkBuffer(), 0, type == IndexSizeType::Index16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
	}

	void VulkanCommandBuffer::DrawIndexedCore(const uint32 indexCount, const uint32 indexOffset, const uint32 vertexOffset)
	{
		vkCmdDrawIndexed(mVkCommandBuffer, indexCount, 1, indexOffset, vertexOffset, 0);
	}

	void VulkanCommandBuffer::DispatchComputeCore(const uint32 groupCountX, const uint32 groupCountY, const uint32 groupCountZ)
	{
		vkCmdDispatch(mVkCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void VulkanCommandBuffer::BindPipelineCore(Pipeline* pipeline)
	{
		VulkanPipeline* pPipeline = static_cast<VulkanPipeline*>(pipeline);

		vkCmdBindPipeline(mVkCommandBuffer, pPipeline->GetVkPipelineBindPoint(), pPipeline->GetVkPipeline());
	}

	void VulkanCommandBuffer::BeginRenderPassCore(RenderPass* renderPass, const Vector4f& clearColor, const byte ColorValueCount, const double clearDepth, const double clearStencil)
	{

	}

	void VulkanCommandBuffer::EndRenderPassCore()
	{
		vkCmdEndRenderPass(mVkCommandBuffer);
	}

	void VulkanCommandBuffer::SetViewportsCore(ViewportDesc* viewports, const byte amount)
	{
		VkViewport* pVkViewports = new VkViewport[amount];

		for (byte i = 0; i < amount; i++)
		{
			pVkViewports[i].x = viewports[i].OffsetSize.x;
			pVkViewports[i].y = viewports[i].OffsetSize.y;
			pVkViewports[i].width = viewports[i].ImageSize.x;
			pVkViewports[i].height = viewports[i].ImageSize.y;
			pVkViewports[i].minDepth = viewports[i].DepthRange.x;
			pVkViewports[i].maxDepth = viewports[i].DepthRange.y;
		}

		vkCmdSetViewport(mVkCommandBuffer, 0, amount, pVkViewports);

		delete[] pVkViewports;
	}

	void VulkanCommandBuffer::SetScissorsCore(ScissorDesc* scissors, const byte amount)
	{
		VkRect2D* pVkScissors = new VkRect2D[amount];

		for (byte i = 0; i < amount; i++)
		{
			pVkScissors[i].offset = { scissors[i].OffsetSize.x, scissors[i].OffsetSize.y };
			pVkScissors[i].extent = { scissors[i].ImageSize.x, scissors[i].ImageSize.y };
		}

		vkCmdSetScissor(mVkCommandBuffer, 0, amount, pVkScissors);

		delete[] pVkScissors;
	}

	void VulkanCommandBuffer::CopyBufferToBufferCore(const GraphicsBuffer* srcBuffer, const GraphicsBuffer* dstBuffer, const BufferBufferCopyDesc& desc)
	{
		VkBufferCopy copyRegion = {};
		copyRegion.size = desc.SizeInBytes;
		copyRegion.dstOffset = desc.DestinationOffsetInBytes;
		copyRegion.srcOffset = desc.SourceOffsetInBytes;

		const VulkanGraphicsBuffer* pSrcBuffer = static_cast<const VulkanGraphicsBuffer*>(srcBuffer);
		const VulkanGraphicsBuffer* pDstBuffer = static_cast<const VulkanGraphicsBuffer*>(dstBuffer);

		vkCmdCopyBuffer(mVkCommandBuffer, pSrcBuffer->GetVkBuffer(), pDstBuffer->GetVkBuffer(), 1, &copyRegion);
	}

	void VulkanCommandBuffer::CopyBufferToTextureCore(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc)
	{
		VkBufferImageCopy copyBufferToImageInfo = {};
		copyBufferToImageInfo.bufferOffset = desc.BufferOffsetInBytes;
		copyBufferToImageInfo.bufferRowLength = 0;
		copyBufferToImageInfo.bufferImageHeight = 0;

		copyBufferToImageInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyBufferToImageInfo.imageSubresource.mipLevel = desc.TargetMipIndex;
		copyBufferToImageInfo.imageSubresource.baseArrayLayer = desc.TargetArrayIndex;
		copyBufferToImageInfo.imageSubresource.layerCount = dstTexture->GetArraySize();

		copyBufferToImageInfo.imageOffset = { static_cast<int32>(desc.TextureOffsetX), static_cast<int32>(desc.TextureOffsetY), static_cast<int32>(desc.TextureOffsetZ) };
		copyBufferToImageInfo.imageExtent = { static_cast<uint32>(desc.Width), static_cast<uint32>(desc.Height), static_cast<uint32>(desc.Depth) };

		const VulkanGraphicsBuffer* pSrcBuffer = static_cast<const VulkanGraphicsBuffer*>(srcBuffer);
		const VulkanTexture* pDstTexture = static_cast<const VulkanTexture*>(dstTexture);

		vkCmdCopyBufferToImage(mVkCommandBuffer, pSrcBuffer->GetVkBuffer(), pDstTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyBufferToImageInfo);
	}

	void VulkanCommandBuffer::CopyTextureToTextureCore(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc)
	{
		if (!desc.IsBlit)
		{
			VkImageCopy copyRegion = {};
			copyRegion.srcSubresource.aspectMask = VulkanTextureUtils::GetVkTextureAspects(desc.SourceAspect);
			copyRegion.srcSubresource.mipLevel = desc.SourceMipIndex;
			copyRegion.srcSubresource.baseArrayLayer = desc.SourceArrayIndex;
			copyRegion.srcSubresource.layerCount = srcTexture->GetArraySize();
			copyRegion.srcOffset = { static_cast<int32>(desc.SourceOffsetX), static_cast<int32>(desc.SourceOffsetY), static_cast<int32>(desc.SourceOffsetZ) };
			copyRegion.dstOffset = { static_cast<int32>(desc.DestinationOffsetX), static_cast<int32>(desc.DestinationOffsetY), static_cast<int32>(desc.DestinationOffsetZ) };
			copyRegion.dstSubresource.aspectMask = VulkanTextureUtils::GetVkTextureAspects(desc.DestinationAspect);
			copyRegion.dstSubresource.mipLevel = desc.DestinationMipIndex;
			copyRegion.dstSubresource.baseArrayLayer = desc.DestinationArrayIndex;
			copyRegion.dstSubresource.layerCount = dstTexture->GetArraySize();
			copyRegion.extent = { srcTexture->GetImageSize().x, srcTexture->GetImageSize().y, srcTexture->GetImageSize().z };

			const VulkanTexture* pSrcTexture = static_cast<const VulkanTexture*>(srcTexture);
			const VulkanTexture* pDstTexture = static_cast<const VulkanTexture*>(dstTexture);

			vkCmdCopyImage(mVkCommandBuffer, pSrcTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, pDstTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
		}
		else
		{
			VkImageBlit blitInfo = {};
			blitInfo.srcOffsets[0] = { (int)desc.SourceOffsetX,(int)desc.SourceOffsetY,(int)desc.SourceOffsetZ };
			blitInfo.srcSubresource.aspectMask = VulkanTextureUtils::GetVkTextureAspects(desc.SourceAspect);
			blitInfo.srcSubresource.baseArrayLayer = desc.SourceArrayIndex;
			blitInfo.srcSubresource.layerCount = srcTexture->GetMipLevels();
			blitInfo.srcSubresource.mipLevel = desc.SourceMipIndex;

			blitInfo.dstOffsets[0] = { (int)desc.DestinationOffsetX,(int)desc.DestinationOffsetY,(int)desc.DestinationOffsetZ };
			blitInfo.dstSubresource.aspectMask = VulkanTextureUtils::GetVkTextureAspects(desc.DestinationAspect);
			blitInfo.dstSubresource.baseArrayLayer = desc.DestinationArrayIndex;
			blitInfo.dstSubresource.layerCount = dstTexture->GetMipLevels();
			blitInfo.dstSubresource.mipLevel = desc.DestinationMipIndex;

			vkCmdBlitImage(mVkCommandBuffer, static_cast<const VulkanTexture*>(srcTexture)->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, static_cast<const VulkanTexture*>(dstTexture)->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blitInfo, VK_FILTER_LINEAR);
		}
	}

	void VulkanCommandBuffer::SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
	}

	void VulkanCommandBuffer::SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
	}

	void VulkanCommandBuffer::CommitResourceSetsCore(DescriptorSet** ppSets, const byte amount)
	{
		VkDescriptorSet* pSets = new VkDescriptorSet[amount];
		VulkanPipeline* pPipeline = static_cast<VulkanPipeline*>(GetBoundPipeline());

		for (byte i = 0; i < amount; i++)
		{
			pSets[i] = static_cast<VulkanDescriptorSet*>(ppSets[i])->GetVkDescriptorSet();
		}

		vkCmdBindDescriptorSets(mVkCommandBuffer, pPipeline->GetVkPipelineBindPoint(), pPipeline->GetVkPipelineLayout(), 0, amount, pSets, 0, nullptr);

		delete[] pSets;
	}
}