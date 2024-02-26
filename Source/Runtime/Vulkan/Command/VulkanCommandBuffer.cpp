#include "VulkanCommandBuffer.h"

#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Buffer/VulkanGraphicsBuffer.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>

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
	}

	void VulkanCommandBuffer::CopyBufferToTextureCore(const GraphicsBuffer* srcBuffer, const Texture* dstTexture, const BufferTextureCopyDesc& desc)
	{
	}

	void VulkanCommandBuffer::CopyTextureToTextureCore(const Texture* srcTexture, const Texture* dstTexture, const TextureTextureCopyDesc& desc)
	{
	}

	void VulkanCommandBuffer::SetTextureMemoryBarrierCore(const Texture* pTexture, const TextureMemoryBarrierDesc& desc)
	{
	}

	void VulkanCommandBuffer::SetBufferMemoryBarrierCore(const GraphicsBuffer* pBuffer, const BufferMemoryBarrierDesc& desc)
	{
	}

	void VulkanCommandBuffer::CommitResourceSetsCore(DescriptorSet** ppSets, const byte amount)
	{
	}
}