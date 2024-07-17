#include "VDevice.h"

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

		//Check if the device supports the extensions
		u32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(mAdapter, nullptr, &extensionCount, nullptr);
		DArray<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(mAdapter, nullptr, &extensionCount, availableExtensions.data());

		// Get all the device features related to adapter
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(mAdapter, &deviceFeatures);

		// Add bindless descriptor support
		VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = {};
		descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
		descriptorIndexingFeatures.pNext = nullptr;
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
		return SharedPtr<CmdPool>();
	}

	SharedPtr<CmdBuffer> VDevice::CreateCommandBufferImpl(const CmdBufferDesc& desc)
	{
		return SharedPtr<CmdBuffer>();
	}
}