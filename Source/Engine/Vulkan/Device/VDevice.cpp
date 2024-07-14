#include "VDevice.h"

#include <Engine/Vulkan/Instance/VInstance.h>

#define QUEUE_COUNT 1

namespace Hollow
{
	VDevice::VDevice(SharedInstance pInstance) : GraphicsDevice(pInstance), mAdapter(pInstance->GetVkAdapter()), 
		mInstance(pInstance->GetVkInstance()), mDevice(VK_NULL_HANDLE)
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
				CORE_LOG(HE_INFO, "VDevice", "Graphics Queue: { Index:%d, Capacity:%d, RequestedCount:%d }", index, prop.queueCount, mGraphicsQueueFamily.RequestedCount);
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
			{
				mComputeQueueFamily.FamilyIndex = index;
				mComputeQueueFamily.QueueCapacity = prop.queueCount;
				mComputeQueueFamily.RequestedCount = QUEUE_COUNT > prop.queueCount ? prop.queueCount : QUEUE_COUNT;
				CORE_LOG(HE_INFO, "VDevice", "Compute  Queue: { Index:%d, Capacity:%d,  RequestedCount:%d }", index, prop.queueCount, mComputeQueueFamily.RequestedCount);
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
			{
				mTransferQueueFamily.FamilyIndex = index;
				mTransferQueueFamily.QueueCapacity = prop.queueCount;
				mTransferQueueFamily.RequestedCount = QUEUE_COUNT > prop.queueCount ? prop.queueCount : QUEUE_COUNT;
				CORE_LOG(HE_INFO, "VDevice", "Transfer Queue: { Index:%d, Capacity:%d,  RequestedCount:%d }", index, prop.queueCount, mTransferQueueFamily.RequestedCount);
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

	void VDevice::Shutdown()
	{
	}

	SharedPtr<GraphicsQueue> VDevice::CreateQueueImpl(const GraphicsQueueDesc& desc)
	{
		return SharedPtr<GraphicsQueue>();
	}
}
