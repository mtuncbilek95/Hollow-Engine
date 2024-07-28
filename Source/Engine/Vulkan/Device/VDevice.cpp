#include "VDevice.h"
#include "../Descriptor/VDescriptorUtils.h"

#include <Engine/Vulkan/Instance/VInstance.h>
#include <Engine/Vulkan/Queue/VQueue.h>
#include <Engine/Vulkan/Memory/VMemory.h>
#include <Engine/Vulkan/Texture/VTextureImage.h>
#include <Engine/Vulkan/Texture/VTextureView.h>
#include <Engine/Vulkan/Sampler/VSampler.h>
#include <Engine/Vulkan/Buffer/VBuffer.h>
#include <Engine/Vulkan/Shader/VShader.h>
#include <Engine/Vulkan/Swapchain/VSwapchain.h>
#include <Engine/Vulkan/Descriptor/VDescriptorLayout.h>
#include <Engine/Vulkan/Descriptor/VDescriptorPool.h>
#include <Engine/Vulkan/Descriptor/VDescriptorSet.h>
#include <Engine/Vulkan/Pipeline/VPipeline.h>
#include <Engine/Vulkan/Sync/VFence.h>
#include <Engine/Vulkan/Sync/VSemaphore.h>
#include <Engine/Vulkan/Command/VCmdPool.h>
#include <Engine/Vulkan/Command/VCmdBuffer.h>

#define QUEUE_COUNT 1

namespace Hollow
{
	VDevice::VDevice(WeakInstance pInstance) : GraphicsDevice(pInstance), mAdapter(pInstance.lock()->GetVkAdapter()),
		mInstance(pInstance.lock()->GetVkInstance()), mDevice(VK_NULL_HANDLE)
	{
		CORE_ASSERT(mAdapter != VK_NULL_HANDLE, "VDevice", "Vulkan adapter is null");
		CORE_ASSERT(mInstance != VK_NULL_HANDLE, "VDevice", "Vulkan instance is null");

		// Get the queue family count
		u32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mAdapter, &queueFamilyCount, nullptr);
		CORE_ASSERT(queueFamilyCount > 0, "VDevice", "No queue families found");

		// Get the queue families
		DArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mAdapter, &queueFamilyCount, queueFamilies.data());

		// Find the queue families for graphics, compute and transfer
		i32 index = 0;
		for (auto& prop : queueFamilies)
		{
			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsQueueFamily.FamilyIndex == 255)
			{
				mGraphicsQueueFamily.FamilyIndex = index;
				mGraphicsQueueFamily.QueueCapacity = prop.queueCount;
				mGraphicsQueueFamily.RequestedCount = QUEUE_COUNT > prop.queueCount ? prop.queueCount : QUEUE_COUNT;
				CORE_LOG(HE_INFO, "Graphics Queue: { Index:%d, Capacity:%d, RequestedCount:%d }", index, prop.queueCount, mGraphicsQueueFamily.RequestedCount);
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
			{
				mComputeQueueFamily.FamilyIndex = index;
				mComputeQueueFamily.QueueCapacity = prop.queueCount;
				mComputeQueueFamily.RequestedCount = QUEUE_COUNT > prop.queueCount ? prop.queueCount : QUEUE_COUNT;
				CORE_LOG(HE_INFO, "Compute  Queue: { Index:%d, Capacity:%d,  RequestedCount:%d }", index, prop.queueCount, mComputeQueueFamily.RequestedCount);
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
			{
				mTransferQueueFamily.FamilyIndex = index;
				mTransferQueueFamily.QueueCapacity = prop.queueCount;
				mTransferQueueFamily.RequestedCount = QUEUE_COUNT > prop.queueCount ? prop.queueCount : QUEUE_COUNT;
				CORE_LOG(HE_INFO, "Transfer Queue: { Index:%d, Capacity:%d,  RequestedCount:%d }", index, prop.queueCount, mTransferQueueFamily.RequestedCount);
			}
			index++;
		}

		// Queue priorities
		f32 queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		DArray<VkDeviceQueueCreateInfo> queueCreateInfos = {};

		// Graphics Queue Create Info
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
		graphicsQueueCreateInfo.queueCount = mGraphicsQueueFamily.QueueCapacity;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.push_back(graphicsQueueCreateInfo);

		// Compute Queue Create Info
		VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
		computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueCreateInfo.queueFamilyIndex = mComputeQueueFamily.FamilyIndex;
		computeQueueCreateInfo.queueCount = mComputeQueueFamily.QueueCapacity;
		computeQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.push_back(computeQueueCreateInfo);

		// Transfer Queue Create Info
		VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.queueFamilyIndex = mTransferQueueFamily.FamilyIndex;
		transferQueueCreateInfo.queueCount = mTransferQueueFamily.QueueCapacity;
		transferQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.push_back(transferQueueCreateInfo);

		// Add all the wanted device extensions
		DArray<const char*> extensions;
		extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		extensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_EXTENSION_NAME);

		//Check if the device supports the extensions
		u32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(mAdapter, nullptr, &extensionCount, nullptr);
		DArray<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(mAdapter, nullptr, &extensionCount, availableExtensions.data());

		// Get all the device features related to adapter
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(mAdapter, &deviceFeatures);

		VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT dynamicRenderingUnusedAttachmentsFeatures = {};
		dynamicRenderingUnusedAttachmentsFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT;
		dynamicRenderingUnusedAttachmentsFeatures.pNext = nullptr;
		dynamicRenderingUnusedAttachmentsFeatures.dynamicRenderingUnusedAttachments = VK_TRUE;

		// Add bindless descriptor support
		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = {};
		descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext = &dynamicRenderingUnusedAttachmentsFeatures;
		descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
		descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;

		// Add dynamic rendering support
		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
		dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeatures.pNext = &descriptorIndexingFeatures;
		dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

		// Create the device
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = (u32)queueCreateInfos.size();
		deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceInfo.enabledExtensionCount = (u32)extensions.size();
		deviceInfo.ppEnabledExtensionNames = extensions.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.flags = VkDeviceCreateFlags();
		deviceInfo.pNext = &dynamicRenderingFeatures;

		CORE_ASSERT(vkCreateDevice(mAdapter, &deviceInfo, nullptr, &mDevice) == VK_SUCCESS, "VDevice", "Failed to create device");

		// Reserve the queues for graphics, compute and transfer and store them in the related families.
		mGraphicsQueueFamily.FreeQueues.reserve(mGraphicsQueueFamily.RequestedCount);
		mComputeQueueFamily.FreeQueues.reserve(mComputeQueueFamily.RequestedCount);
		mTransferQueueFamily.FreeQueues.reserve(mTransferQueueFamily.RequestedCount);

		// Get the graphics queues
		for (u8 i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mDevice, mGraphicsQueueFamily.FamilyIndex, i, &queue);
			mGraphicsQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the compute queues
		for (u8 i = 0; i < mComputeQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mDevice, mComputeQueueFamily.FamilyIndex, i, &queue);
			mComputeQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the transfer queues
		for (u8 i = 0; i < mTransferQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mDevice, mTransferQueueFamily.FamilyIndex, i, &queue);
			mTransferQueueFamily.FreeQueues.push_back(queue);
		}
	}

	SharedPtr<TextureImage> VDevice::CreateSwapchainImage(const TextureImageDesc& desc, VkImage image)
	{
		return MakeShared<VTextureImage>(desc, image, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<TextureView> VDevice::CreateSwapchainImageView(const TextureViewDesc& desc)
	{
		return MakeShared<VTextureView>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<GraphicsQueue> VDevice::CreateQueueImpl(const GraphicsQueueDesc& desc)
	{
		switch (desc.QueueType)
		{
		case GraphicsQueueType::Graphics:
			return MakeShared<VQueue>(desc, mGraphicsQueueFamily.GetFreeQueue(), GetSharedPtrAs<VDevice>());
		case GraphicsQueueType::Compute:
			return MakeShared<VQueue>(desc, mComputeQueueFamily.GetFreeQueue(), GetSharedPtrAs<VDevice>());
		case GraphicsQueueType::Transfer:
			return MakeShared<VQueue>(desc, mTransferQueueFamily.GetFreeQueue(), GetSharedPtrAs<VDevice>());
		default:
			CORE_ASSERT(false, "VDevice", "Unknown queue type");
			return nullptr;
		}
	}

	SharedPtr<GraphicsMemory> VDevice::CreateMemoryImpl(const GraphicsMemoryDesc& desc)
	{
		return MakeShared<VMemory>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<TextureImage> VDevice::CreateTextureImageImpl(const TextureImageDesc& desc)
	{
		return MakeShared<VTextureImage>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<TextureView> VDevice::CreateTextureViewImpl(const TextureViewDesc& desc)
	{
		return MakeShared<VTextureView>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Sampler> VDevice::CreateSamplerImpl(const SamplerDesc& desc)
	{
		return MakeShared<VSampler>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<GraphicsBuffer> VDevice::CreateBufferImpl(const GraphicsBufferDesc& desc)
	{
		return MakeShared<VBuffer>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Shader> VDevice::CreateShaderImpl(const ShaderDesc& desc)
	{
		return MakeShared<VShader>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Swapchain> VDevice::CreateSwapchainImpl(const SwapchainDesc& desc)
	{
		return MakeShared<VSwapchain>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<DescriptorLayout> VDevice::CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc)
	{
		return MakeShared<VDescriptorLayout>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<DescriptorPool> VDevice::CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc)
	{
		return MakeShared<VDescriptorPool>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<DescriptorSet> VDevice::CreateDescriptorSetImpl(const DescriptorSetDesc& desc)
	{
		return MakeShared<VDescriptorSet>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Pipeline> VDevice::CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc)
	{
		return MakeShared<VPipeline>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Fence> VDevice::CreateGraphicsFenceImpl(bool bSignalled)
	{
		return MakeShared<VFence>(bSignalled, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<Semaphore> VDevice::CreateGraphicsSemaphoreImpl()
	{
		return MakeShared<VSemaphore>(GetSharedPtrAs<VDevice>());
	}

	SharedPtr<CmdPool> VDevice::CreateCommandPoolImpl(const CmdPoolDesc& desc)
	{
		return MakeShared<VCmdPool>(desc, GetSharedPtrAs<VDevice>());
	}

	SharedPtr<CmdBuffer> VDevice::CreateCommandBufferImpl(const CmdBufferDesc& desc)
	{
		return MakeShared<VCmdBuffer>(desc, GetSharedPtrAs<VDevice>());
	}

	void VDevice::WaitFencesImpl(WeakPtr<Fence> pFences[], u32 count)
	{
		VkFence fences[32];
		for (u8 i = 0; i < count; i++)
			fences[i] = pFences[i].lock()->GetSharedPtrAs<VFence>()->GetVkFence();

		vkWaitForFences(mDevice, count, fences, VK_TRUE, UINT64_MAX);
	}

	void VDevice::WaitFenceImpl(WeakPtr<Fence> pFence)
	{
		VkFence fence = pFence.lock()->GetSharedPtrAs<VFence>()->GetVkFence();
		vkWaitForFences(mDevice, 1, &fence, VK_TRUE, UINT64_MAX);
	}

	void VDevice::ResetFencesImpl(WeakPtr<Fence> pFences[], u32 count)
	{
		VkFence fences[32];
		for (u8 i = 0; i < count; i++)
			fences[i] = pFences[i].lock()->GetSharedPtrAs<VFence>()->GetVkFence();

		vkResetFences(mDevice, count, fences);
	}

	void VDevice::ResetFenceImpl(WeakPtr<Fence> pFence)
	{
		VkFence fence = pFence.lock()->GetSharedPtrAs<VFence>()->GetVkFence();
		vkResetFences(mDevice, 1, &fence);
	}

	void VDevice::WaitIdleImpl()
	{
		vkDeviceWaitIdle(mDevice);
	}

	void VDevice::WaitQueueIdleImpl(WeakPtr<GraphicsQueue> pQueue)
	{
		vkQueueWaitIdle(pQueue.lock()->GetSharedPtrAs<VQueue>()->GetVkQueue());
	}

	void VDevice::SubmitQueueImpl(WeakPtr<GraphicsQueue> pQueue, WeakPtr<CmdBuffer> pCmdBuffer, u32 cmdCount,
		WeakPtr<Semaphore> pWaitSemaphores[], u32 waitCount, WeakPtr<Semaphore> pSignalSemaphores[], u32 signalCount,
		WeakPtr<Fence> pFence, PipelineStageFlags flags[])
	{
		VkCommandBuffer cmdBuffers[32];
		VkSemaphore waitSemaphores[32];
		VkSemaphore signalSemaphores[32];
		VkPipelineStageFlags waitStages[32];

		for (u8 i = 0; i < cmdCount; i++)
			cmdBuffers[i] = pCmdBuffer.lock()->GetSharedPtrAs<VCmdBuffer>()->GetVkCmdBuffer();

		for (u8 i = 0; i < waitCount; i++)
			waitSemaphores[i] = pWaitSemaphores[i].lock()->GetSharedPtrAs<VSemaphore>()->GetVkSemaphore();

		for (u8 i = 0; i < signalCount; i++)
			signalSemaphores[i] = pSignalSemaphores[i].lock()->GetSharedPtrAs<VSemaphore>()->GetVkSemaphore();

		for (u8 i = 0; i < waitCount; i++)
			waitStages[i] = (VkPipelineStageFlags)flags[i];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = waitCount;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = cmdCount;
		submitInfo.pCommandBuffers = cmdBuffers;
		submitInfo.signalSemaphoreCount = signalCount;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkFence fence = pFence.lock()->GetSharedPtrAs<VFence>()->GetVkFence();

		vkQueueSubmit(pQueue.lock()->GetSharedPtrAs<VQueue>()->GetVkQueue(), 1, &submitInfo, fence);
	}

	void VDevice::UpdateHostBufferImpl(WeakPtr<GraphicsBuffer> pBuffer, const BufferDataUpdateDesc& desc)
	{
		auto pMemory = pBuffer.lock()->GetMemory().lock()->GetSharedPtrAs<VMemory>();
		auto pDst = pBuffer.lock()->GetSharedPtrAs<VBuffer>();

		void* data;
		vkMapMemory(mDevice, pMemory->GetVkDeviceMemory(), pDst->GetAlignedOffset() + desc.OffsetInBytes, desc.Memory.GetSize(), 0, &data);
		memcpy(data, desc.Memory.GetData(), desc.Memory.GetSize());
		vkUnmapMemory(mDevice, pMemory->GetVkDeviceMemory());
	}

	void VDevice::UpdateDescriptorSetImpl(WeakPtr<DescriptorSet> pDescriptorSet, const DescriptorUpdateDesc& desc)
	{
		auto pDst = pDescriptorSet.lock()->GetSharedPtrAs<VDescriptorSet>();

		VkWriteDescriptorSet writeInformations[32];
		VkDescriptorBufferInfo writeBufferInformations[32];
		VkDescriptorImageInfo writeImageInformations[32];
		u32 bufferIndex = 0;
		u32 imageIndex = 0;

		for (u8 i = 0; i < desc.Entries.size(); i++)
		{
			VkWriteDescriptorSet writeInfo = {};
			writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo.dstSet = pDst->GetVkDescriptorSet();
			writeInfo.dstBinding = desc.Entries[i].Binding;
			writeInfo.dstArrayElement = desc.Entries[i].ArrayElement;
			writeInfo.descriptorCount = desc.Entries[i].Count;
			writeInfo.descriptorType = VkUtils::GetVkDescriptorType(desc.Entries[i].Type);
			writeInfo.pNext = nullptr;

			switch (desc.Entries[i].Type)
			{
			case DescriptorType::Sampler:
			{
				auto pSampler = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VSampler>();

				VkDescriptorImageInfo samplerInfo = {};
				samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				samplerInfo.imageView = VK_NULL_HANDLE;
				samplerInfo.sampler = pSampler->GetVkSampler();
				writeImageInformations[imageIndex] = samplerInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				imageIndex++;
				break;
			}
			case DescriptorType::CombinedImageSampler:
			{
				auto pView = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VTextureView>();
				auto pSampler = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VSampler>();

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = pView->GetVkTextureView();
				imageInfo.sampler = pSampler->GetVkSampler();
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				imageIndex++;
				break;
			}
			case DescriptorType::SampledImage:
			{
				auto pView = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VTextureView>();

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = pView->GetVkTextureView();
				imageInfo.sampler = VK_NULL_HANDLE;
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				imageIndex++;
				break;
			}
			case DescriptorType::StorageImage:
			{
				auto pView = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VTextureView>();

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageInfo.imageView = pView->GetVkTextureView();
				imageInfo.sampler = VK_NULL_HANDLE;
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				imageIndex++;
				break;
			}
			case DescriptorType::UniformTexelBuffer:
			{
				// Add code for handling UniformTexelBuffer descriptor type
				break;
			}
			case DescriptorType::StorageTexelBuffer:
			{
				// Add code for handling StorageTexelBuffer descriptor type
				break;
			}
			case DescriptorType::UniformBuffer:
			{
				auto pBuffer = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VBuffer>();

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = pBuffer->GetBuffer();
				bufferInfo.offset = desc.Entries[i].BufferOffset;
				bufferInfo.range = pBuffer->GetTotalSize();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				bufferIndex++;
				break;
			}
			case DescriptorType::StorageBuffer:
			{
				auto pBuffer = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VBuffer>();

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = pBuffer->GetBuffer();
				bufferInfo.offset = desc.Entries[i].BufferOffset;
				bufferInfo.range = pBuffer->GetTotalSize();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				bufferIndex++;
				break;
			}
			case DescriptorType::UniformBufferDynamic:
			{
				auto pBuffer = desc.Entries[i].pResource.lock()->GetSharedPtrAs<VBuffer>();

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = pBuffer->GetBuffer();
				bufferInfo.offset = desc.Entries[i].BufferOffset;
				bufferInfo.range = pBuffer->GetTotalSize();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				bufferIndex++;
				break;
			}
			case DescriptorType::StorageBufferDynamic:
			{
				// Add code for handling StorageBufferDynamic descriptor type
				break;
			}
			case DescriptorType::InputAttachment:
			{
				// Add code for handling InputAttachment descriptor type
				break;
			}
			default:
				CORE_LOG(HE_WARNING, "VDevice", "The DescriptorType that you are using is not valid for UpdateDescriptorSet()");
				break;
			}

			writeInformations[i] = writeInfo;
		}

		vkUpdateDescriptorSets(mDevice, desc.Entries.size(), writeInformations, 0, nullptr);
	}
}