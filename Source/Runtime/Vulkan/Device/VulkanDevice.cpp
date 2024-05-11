#include "VulkanDevice.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Texture/VulkanTextureBuffer.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>

namespace Hollow
{
	VulkanDevice::VulkanDevice(const GraphicsDeviceDesc& desc) : GraphicsDevice(desc), mVkDevice(VK_NULL_HANDLE), mVkPhysicalDevice(VK_NULL_HANDLE), mVkInstance(VK_NULL_HANDLE)
	{
		mVkInstance = std::static_pointer_cast<VulkanInstance>(desc.Instance)->GetVkInstance();

		// Get the physical device count
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr);
		CORE_ASSERT(deviceCount > 0, "VulkanDevice", "No physical devices found");

		// Get the physical devices
		ArrayList<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data());

		// Find the related physical device according to the chosen device
		for (auto& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			String deviceName = deviceProperties.deviceName;

			if (deviceName == mAdapter->ProductName)
			{
				mVkPhysicalDevice = device;
				break;
			}
		}

		CORE_ASSERT(mVkPhysicalDevice != VK_NULL_HANDLE, "VulkanDevice", "No physical device found for the given adapter");

		// Get the device properties

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &deviceProperties);

		CORE_LOG(HE_INFO, "VulkanDevice", "Granularity: %d", deviceProperties.limits.bufferImageGranularity);

		// Get the queue family count
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, nullptr);
		CORE_ASSERT(queueFamilyCount > 0, "VulkanDevice", "No queue families found");

		// Get the queue families
		ArrayList<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
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
		ArrayList<VkDeviceQueueCreateInfo> queueCreateInfos = {};

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
		ArrayList<const char*> deviceExtensions;
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

		CORE_ASSERT(vkCreateDevice(mVkPhysicalDevice, &deviceInfo, nullptr, &mVkDevice) == VK_SUCCESS, "VulkanDevice", "Failed to create logical device");

		// Reserve the queues for graphics, compute and transfer and store them in the related families.
		mGraphicsQueueFamily.FreeQueues.reserve(mGraphicsQueueFamily.RequestedCount);
		mComputeQueueFamily.FreeQueues.reserve(mComputeQueueFamily.RequestedCount);
		mTransferQueueFamily.FreeQueues.reserve(mTransferQueueFamily.RequestedCount);

		// Get the graphics queues
		for (byte i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamily.FamilyIndex, i, &queue);
			mGraphicsQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the compute queues
		for (byte i = 0; i < mComputeQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mComputeQueueFamily.FamilyIndex, i, &queue);
			mComputeQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the transfer queues
		for (byte i = 0; i < mTransferQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mTransferQueueFamily.FamilyIndex, i, &queue);
			mTransferQueueFamily.FreeQueues.push_back(queue);
		}
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureForSwapchain(const TextureDesc& desc, VkImage image)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTexture>(desc, image, device);
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
		for (int i = static_cast<int>(mDeviceObjects.size()) - 1; i >= 0; --i)
		{
			mDeviceObjects[i]->OnShutdown();
			mDeviceObjects[i].reset(); // Release the shared pointer
		}

		if (mOwnedSwapchain != nullptr)
		{
			//mOwnedSwapchain->OnShutdown();
			mOwnedSwapchain.reset();
		}

		if (mVkDevice != VK_NULL_HANDLE)
			vkDestroyDevice(mVkDevice, nullptr);

		mVkInstance = VK_NULL_HANDLE;
		mVkPhysicalDevice = VK_NULL_HANDLE;

		mGraphicsQueueFamily.FamilyIndex = 255;
		mComputeQueueFamily.FamilyIndex = 255;
		mTransferQueueFamily.FamilyIndex = 255;

		CORE_LOG(HE_INFO, "VulkanDevice", "Device shutdown");
	}

	SharedPtr<Swapchain> VulkanDevice::CreateSwapchainImpl(const SwapchainDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanSwapchain>(desc, device);
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureImpl(const TextureDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTexture>(desc, device);
	}

	SharedPtr<TextureBuffer> VulkanDevice::CreateTextureBufferImpl(const TextureBufferDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTextureBuffer>(desc, device);
	}

	SharedPtr<GraphicsMemory> VulkanDevice::CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc)
	{
		return SharedPtr<GraphicsMemory>();
	}

	SharedPtr<Semaphore> VulkanDevice::CreateSyncSemaphoreImpl()
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanSemaphore>(device);
	}

	SharedPtr<Fence> VulkanDevice::CreateSyncFenceImpl(const FenceDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanFence>(desc, device);
	}

	SharedPtr<GraphicsQueue> VulkanDevice::CreateQueueImpl(const GraphicsQueueDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		switch (desc.QueueType)
		{
		case GraphicsQueueType::Graphics:
		{
			if (mGraphicsQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mGraphicsQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free graphics queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Compute:
		{
			if (mComputeQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mComputeQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free compute queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Transfer:
		{
			if (mTransferQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mTransferQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free transfer queue found");
				return nullptr;
			}
		}
		default:
			CORE_LOG(HE_ERROR, "VulkanDevice", "Invalid queue type");
			return nullptr;
		}
	}

	SharedPtr<Sampler> VulkanDevice::CreateSamplerImpl(const SamplerDesc& desc)
	{
		return SharedPtr<Sampler>();
	}

	SharedPtr<Shader> VulkanDevice::CreateShaderImpl(const ShaderDesc& desc)
	{
		return SharedPtr<Shader>();
	}

	SharedPtr<RenderPass> VulkanDevice::CreateRenderPassImpl(const RenderPassDesc& desc)
	{
		return SharedPtr<RenderPass>();
	}

	SharedPtr<Pipeline> VulkanDevice::CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc)
	{
		return SharedPtr<Pipeline>();
	}

	SharedPtr<CommandBuffer> VulkanDevice::CreateCommandBufferImpl(const CommandBufferDesc& desc)
	{
		return SharedPtr<CommandBuffer>();
	}

	SharedPtr<CommandPool> VulkanDevice::CreateCommandPoolImpl(const CommandPoolDesc& desc)
	{
		return SharedPtr<CommandPool>();
	}

	SharedPtr<GraphicsBuffer> VulkanDevice::CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanBuffer>(desc, device);
	}

	SharedPtr<DescriptorSet> VulkanDevice::CreateDescriptorSetImpl(const DescriptorSetDesc& desc)
	{
		return SharedPtr<DescriptorSet>();
	}

	SharedPtr<DescriptorPool> VulkanDevice::CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc)
	{
		return SharedPtr<DescriptorPool>();
	}

	SharedPtr<DescriptorLayout> VulkanDevice::CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc)
	{
		return SharedPtr<DescriptorLayout>();
	}

	void VulkanDevice::WaitForSemaphoreImpl(SharedPtr<Semaphore> ppSemaphores[], uint32 amount)
	{
	}

	void VulkanDevice::WaitForFenceImpl(SharedPtr<Fence> ppFences[], uint32 amount)
	{
	}

	void VulkanDevice::ResetFencesImpl(SharedPtr<Fence> ppFences[], uint32 amount)
	{
	}

	void VulkanDevice::UpdateBufferDataImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc)
	{
	}

	void VulkanDevice::WaitForIdleImpl()
	{
	}

	void VulkanDevice::WaitQueueIdleImpl(SharedPtr<GraphicsQueue> pQueue)
	{
	}

	void VulkanDevice::SubmitToQueueImpl(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], uint32 amount, 
		SharedPtr<Semaphore> ppWaitSemaphores[], uint32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], 
		uint32 signalSemaphoreCount, SharedPtr<Fence> pFence)
	{
	}

	void VulkanDevice::CopyDescriptorSetImpl(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc)
	{
	}

	void VulkanDevice::UpdateDescriptorSetImpl(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc)
	{
	}
}
