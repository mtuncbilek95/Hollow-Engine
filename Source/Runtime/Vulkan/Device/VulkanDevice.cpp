#include "VulkanDevice.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Texture/VulkanTextureView.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Shader/VulkanShader.h>

namespace Hollow
{
	VulkanDevice::VulkanDevice(const GraphicsDeviceDesc& desc) : GraphicsDevice(desc), mVkPhysicalDevice(VK_NULL_HANDLE),
		mVkDevice(VK_NULL_HANDLE), mVkInstance(VK_NULL_HANDLE)
	{
		// Get the instance from desc
		mVkInstance = reinterpret_cast<VulkanInstance*>(desc.Instance)->GetVkInstance();

		// Get the physical device count
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr);
		DEV_ASSERT(deviceCount > 0, "VulkanDevice", "No physical devices found");

		// Get the physical devices
		Array<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data());

		// Find the related physical device according to the chosen device
		for (auto& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			if (strcmp(deviceProperties.deviceName, mGraphicsAdapter.ProductName.c_str()) == 0)
			{
				mVkPhysicalDevice = device;
				break;
			}
		}

		DEV_ASSERT(mVkPhysicalDevice != VK_NULL_HANDLE, "VulkanDevice", "No physical device found for the given adapter");

		// Get the queue family count
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, nullptr);
		DEV_ASSERT(queueFamilyCount > 0, "VulkanDevice", "No queue families found");

		// Get the queue families
		Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

		// Find the queue families for graphics, compute and transfer
		int index = 0;
		for (auto& prop : queueFamilies)
		{
			if (mGraphicsQueueFamily.FamilyIndex == 255 || mComputeQueueFamily.FamilyIndex == 255 || mTransferQueueFamily.FamilyIndex == 255)
				CORE_LOG(HE_WARNING, "VulkanDevice", "Queue Family Index: %d", index);

			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Graphics Found. Count: %d", prop.queueCount);
				mGraphicsQueueFamily.FamilyIndex = index;
				mGraphicsQueueFamily.QueueCapacity = prop.queueCount;
				mGraphicsQueueFamily.RequestedCount = desc.GraphicsQueueCount > prop.queueCount ? prop.queueCount : desc.GraphicsQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Compute Found. Count: %d", prop.queueCount);
				mComputeQueueFamily.FamilyIndex = index;
				mComputeQueueFamily.QueueCapacity = prop.queueCount;
				mComputeQueueFamily.RequestedCount = desc.ComputeQueueCount > prop.queueCount ? prop.queueCount : desc.ComputeQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Transfer Found. Count: %d", prop.queueCount);
				mTransferQueueFamily.FamilyIndex = index;
				mTransferQueueFamily.QueueCapacity = prop.queueCount;
				mTransferQueueFamily.RequestedCount = desc.TransferQueueCount > prop.queueCount ? prop.queueCount : desc.TransferQueueCount;
			}
			index++;
		}

		// Queue priorities
		float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		Array<VkDeviceQueueCreateInfo> queueCreateInfos = {};

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

		// Device Extensions
		Array<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// Device Features
		VkPhysicalDeviceFeatures deviceFeatures = {};
		vkGetPhysicalDeviceFeatures(mVkPhysicalDevice, &deviceFeatures);

		// Device Info
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
		deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.flags = VkDeviceCreateFlags();
		deviceInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateDevice(mVkPhysicalDevice, &deviceInfo, nullptr, &mVkDevice) == VK_SUCCESS, "VulkanDevice", "Failed to create logical device");

		// Reserve the queues for graphics, compute and transfer and store them in the related families.
		mGraphicsQueueFamily.FreeQueues.reserve(mGraphicsQueueFamily.RequestedCount);
		mComputeQueueFamily.FreeQueues.reserve(mComputeQueueFamily.RequestedCount);
		mTransferQueueFamily.FreeQueues.reserve(mTransferQueueFamily.RequestedCount);

		// Get the graphics queues
		for (uint32 i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamily.FamilyIndex, i, &queue);
			mGraphicsQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the compute queues
		for (uint32 i = 0; i < mComputeQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mComputeQueueFamily.FamilyIndex, i, &queue);
			mComputeQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the transfer queues
		for (uint32 i = 0; i < mTransferQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mTransferQueueFamily.FamilyIndex, i, &queue);
			mTransferQueueFamily.FreeQueues.push_back(queue);
		}
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureForSwapchain(const TextureDesc& desc, VkImage image)
	{
		return std::make_shared<VulkanTexture>(desc, image, this);
	}

	uint32 VulkanDevice::GetQueueFamilyIndex(GraphicsQueueType type) const
	{
		switch (type)
		{
		case GraphicsQueueType::Graphics:
			return mGraphicsQueueFamily.FamilyIndex;
		case GraphicsQueueType::Compute:
			return mComputeQueueFamily.FamilyIndex;
		case GraphicsQueueType::Transfer:
			return mTransferQueueFamily.FamilyIndex;
		default:
			return 255;
		}
	}

	void VulkanDevice::OnShutdown()
	{
		if (mVkDevice != VK_NULL_HANDLE)
			vkDestroyDevice(mVkDevice, nullptr);

		mVkDevice = VK_NULL_HANDLE;
		mVkInstance = VK_NULL_HANDLE;
		mVkPhysicalDevice = VK_NULL_HANDLE;
	}

	SharedPtr<Swapchain> VulkanDevice::CreateSwapchainImpl(const SwapchainDesc& desc)
	{
		return std::make_shared<VulkanSwapchain>(desc, this);
	}

	SharedPtr<GraphicsQueue> VulkanDevice::CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc)
	{
		switch (desc.QueueType)
		{
		case GraphicsQueueType::Graphics:
		{
			if (mGraphicsQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mGraphicsQueueFamily.GetFreeQueue(), this);
			else
			{
				CORE_LOG(HE_FATAL, "VulkanDevice", "No free graphics queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Compute:
		{
			if (mComputeQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mComputeQueueFamily.GetFreeQueue(), this);
			else
			{
				CORE_LOG(HE_FATAL, "VulkanDevice", "No free compute queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Transfer:
		{
			if (mTransferQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mTransferQueueFamily.GetFreeQueue(), this);
			else
			{
				CORE_LOG(HE_FATAL, "VulkanDevice", "No free transfer queue found");
				return nullptr;
			}
		}
		default:
			CORE_LOG(HE_FATAL, "VulkanDevice", "Invalid queue type");
			return nullptr;
		}
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureImpl(const TextureDesc& desc)
	{
		return std::make_shared<VulkanTexture>(desc, this);
	}

	SharedPtr<TextureView> VulkanDevice::CreateTextureViewImpl(const TextureViewDesc& desc)
	{
		return std::make_shared<VulkanTextureView>(desc, this);
	}

	SharedPtr<Semaphore> VulkanDevice::CreateSyncSemaphoreImpl()
	{
		return std::make_shared<VulkanSemaphore>(this);
	}

	SharedPtr<Fence> VulkanDevice::CreateFenceImpl(const FenceDesc& desc)
	{
		return std::make_shared<VulkanFence>(desc, this);
	}

	SharedPtr<Shader> VulkanDevice::CreateShaderImpl(const ShaderDesc& desc)
	{
		return std::make_shared<VulkanShader>(desc, this);
	}

	void VulkanDevice::WaitForFenceImpl(Fence** ppFences, uint32 amount)
	{
		VkFence fences[32];
		for (uint32 i = 0; i < amount; i++)
			fences[i] = reinterpret_cast<VulkanFence*>(ppFences[i])->GetVkFence();

		vkWaitForFences(mVkDevice, amount, fences, VK_TRUE, UINT64_MAX);
	}

	void VulkanDevice::ResetFencesImpl(Fence** ppFences, uint32 amount)
	{
		VkFence fences[32];
		for (uint32 i = 0; i < amount; i++)
			fences[i] = reinterpret_cast<VulkanFence*>(ppFences[i])->GetVkFence();

		vkResetFences(mVkDevice, amount, fences);
	}
}