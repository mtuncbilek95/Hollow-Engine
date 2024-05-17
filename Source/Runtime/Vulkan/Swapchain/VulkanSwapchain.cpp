#include "VulkanSwapchain.h"

#ifdef HOLLOW_PLATFORM_WINDOWS

#include <Runtime/Win32/Win32Window.h>
#include <vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <Runtime/Window/WindowManager.h>

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchainUtils.h>
#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>

#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>

namespace Hollow
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainDesc& desc, SharedPtr<VulkanDevice> device) : Swapchain(desc, device), 
		mVkSurface(VK_NULL_HANDLE), mVkSwapchain(VK_NULL_HANDLE)
	{
		auto vulkanDevice = std::static_pointer_cast<VulkanDevice>(device);

		mVkDevice = vulkanDevice->GetVkDevice();
		mVkPhysicalDevice = vulkanDevice->GetVkPhysicalDevice();
		mVkInstance = vulkanDevice->GetVkInstance();

		// Create the surface
#ifdef HOLLOW_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = reinterpret_cast<Win32Window*>(WindowManager::GetInstanceAPI().GetDefaultWindow().get())->GetInstance();
		surfaceInfo.hwnd = reinterpret_cast<Win32Window*>(WindowManager::GetInstanceAPI().GetDefaultWindow().get())->GetHandle();
		surfaceInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateWin32SurfaceKHR(mVkInstance, &surfaceInfo, nullptr, &mVkSurface) == VK_SUCCESS, "VulkanSwapchain", "Failed to create surface");
#endif

		// Get the surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVkPhysicalDevice, mVkSurface, &surfaceCapabilities) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface capabilities");

#ifdef HOLLOW_DEBUG
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Min Image Count: %d", surfaceCapabilities.minImageCount);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Count: %d", surfaceCapabilities.maxImageCount);
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Current Extent: %d x %d", surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Min Image Extent: %d x %d", surfaceCapabilities.minImageExtent.width, surfaceCapabilities.minImageExtent.height);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Extent: %d x %d", surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Array Layers: %d", surfaceCapabilities.maxImageArrayLayers);
#endif

		// Get the surface format count
		uint32 formatCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, nullptr) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats");
		CORE_ASSERT(formatCount > 0, "VulkanSwapchain", "No surface formats found");

		// Get the surface formats
		ArrayList<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats");

		// Check if requested format is supported.
		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == VulkanTextureUtils::GetVkTextureFormat(desc.SwapchainImageFormat) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested format is supported");
				colorSpace = format.colorSpace;
				break;
			}
		}

		// Check if requested image size is supported
		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (desc.ImageSize.x > surfaceCapabilities.maxImageExtent.width || desc.ImageSize.y > surfaceCapabilities.maxImageExtent.height)
			{
				CORE_LOG(HE_WARNING, "VulkanSwapchain", "Requested image size is not supported. Using the current extent size.");
				SetNewImageSize({ surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height });
			}
			else
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested image size is supported");
		}

		// Get the present mode count
		uint32 presentModeCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");
		CORE_ASSERT(presentModeCount > 0, "VulkanSwapchain", "No present modes found");

		// Get the present modes
		ArrayList<VkPresentModeKHR> presentModes(presentModeCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");

		// Check if the requested present mode is supported
		for (auto& mode : presentModes)
		{
			if (mode == VulkanSwapchainUtils::GetVkPresentMode(desc.VSync))
			{
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested present mode is supported");
				break;
			}
		}

		// Get the present queue family info and check if it supports present
		auto pVkQueue = std::static_pointer_cast<VulkanQueue>(desc.pQueue);
		uint32 presentQueueFamilyIndex = pVkQueue->GetQueueFamilyIndex();
		CORE_ASSERT(presentQueueFamilyIndex != UINT32_MAX, "VulkanSwapchain", "Failed to get present queue family index");
		VkBool32 presentSupport = false;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mVkPhysicalDevice, presentQueueFamilyIndex, mVkSurface, &presentSupport) == VK_SUCCESS,
			"VulkanSwapchain", "Failed to get present support");

		CORE_ASSERT(presentSupport, "VulkanSwapchain", "Present support not found");

		// Create the swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mVkSurface;
		swapchainInfo.minImageCount = desc.BufferCount;
		swapchainInfo.imageFormat = VulkanTextureUtils::GetVkTextureFormat(desc.SwapchainImageFormat);
		swapchainInfo.imageColorSpace = colorSpace;
		swapchainInfo.imageExtent = { GetImageSize().x, GetImageSize().y };
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VulkanTextureUtils::GetVkTextureUsageFlags(desc.SwapchainUsage);
		swapchainInfo.imageSharingMode = VulkanCoreUtils::GetVkSharingMode(desc.SwapchainMode);
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &presentQueueFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = VulkanSwapchainUtils::GetVkPresentMode(desc.VSync);
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateSwapchainKHR(mVkDevice, &swapchainInfo, nullptr, &mVkSwapchain) == VK_SUCCESS, "VulkanSwapchain", "Failed to create swapchain");

		// After creating the swapchain, get the images from the swapchain
		// Create the images without initializing them since we already have
		// VkImage from the swapchain

		uint32 imageCount = 0;
		CORE_ASSERT(vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchain, &imageCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");
		CORE_ASSERT(imageCount > 0, "VulkanSwapchain", "No swapchain images found");

		ArrayList<VkImage> images(imageCount);
		CORE_ASSERT(vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchain, &imageCount, images.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");

		// Nevertheless, we need to fill the VulkanTexture data
		for (uint32 i = 0; i < imageCount; i++)
		{
			TextureDesc textureDesc = {};
			textureDesc.ArraySize = 1;
			textureDesc.ImageFormat = desc.SwapchainImageFormat;
			textureDesc.ImageSize = { GetImageSize().x, GetImageSize().y, 1 };
			printf("Image Size: %d x %d\n", GetImageSize().x, GetImageSize().y);
			textureDesc.MipLevels = 1;
			textureDesc.SampleCount = TextureSampleCount::Sample1;
			textureDesc.Type = TextureType::Texture2D;
			textureDesc.Usage = desc.SwapchainUsage;

			auto pTexture = device->CreateTextureForSwapchain(textureDesc, images[i]);
			AddTexture(pTexture);

			// After creating the VulkanTexture, we need to create the VulkanTextureView
			TextureBufferDesc viewDesc = {};
			viewDesc.pTexture = pTexture;
			viewDesc.ArrayLayer = 0;
			viewDesc.MipLevel = 0;
			viewDesc.AspectFlags = TextureAspectFlags::ColorAspect;

			auto pTextureView = device->CreateTextureBuffer(viewDesc);
			AddTextureBuffer(pTextureView);
		}
	}

	void VulkanSwapchain::OnShutdown() noexcept
	{
		if (mVkSwapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(mVkDevice, mVkSwapchain, nullptr);
		}

		if (mVkSurface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(mVkInstance, mVkSurface, nullptr);
		}

		mVkSwapchain = VK_NULL_HANDLE;
		mVkSurface = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
		mVkPhysicalDevice = VK_NULL_HANDLE;
		mVkInstance = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanSwapchain", "Swapchain shutdown successfully");
	}

	void VulkanSwapchain::ResizeImpl(const Vector2u& newSize)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetOwnerDevice());

		// Free swapchain and surface
		if (mVkSwapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(mVkDevice, mVkSwapchain, nullptr);

		if (mVkSurface != VK_NULL_HANDLE)
			vkDestroySurfaceKHR(mVkInstance, mVkSurface, nullptr);

		// Create the surface
#ifdef HOLLOW_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = reinterpret_cast<Win32Window*>(WindowManager::GetInstanceAPI().GetDefaultWindow().get())->GetInstance();
		surfaceInfo.hwnd = reinterpret_cast<Win32Window*>(WindowManager::GetInstanceAPI().GetDefaultWindow().get())->GetHandle();
		surfaceInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateWin32SurfaceKHR(mVkInstance, &surfaceInfo, nullptr, &mVkSurface) == VK_SUCCESS, "VulkanSwapchain", "Failed to create surface");
#endif

		// Get the surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVkPhysicalDevice, mVkSurface, &surfaceCapabilities) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface capabilities");

#ifdef HOLLOW_DEBUG
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Min Image Count: %d", surfaceCapabilities.minImageCount);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Count: %d", surfaceCapabilities.maxImageCount);
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Current Extent: %d x %d", surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);
		CORE_LOG(HE_INFO, "DEBUG INFO SURFACE", "Min Image Extent: %d x %d", surfaceCapabilities.minImageExtent.width, surfaceCapabilities.minImageExtent.height);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Extent: %d x %d", surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
		CORE_LOG(HE_WARNING, "DEBUG INFO SURFACE", "Max Image Array Layers: %d", surfaceCapabilities.maxImageArrayLayers);
#endif

		// Get the surface format count
		uint32 formatCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, nullptr) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats");
		CORE_ASSERT(formatCount > 0, "VulkanSwapchain", "No surface formats found");

		// Get the surface formats
		ArrayList<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats");

		// Check if requested format is supported.
		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == VulkanTextureUtils::GetVkTextureFormat(GetSwapchainFormat()) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested format is supported");
				colorSpace = format.colorSpace;
				break;
			}
		}

		// Check if requested image size is supported
		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (newSize.x > surfaceCapabilities.maxImageExtent.width || newSize.y > surfaceCapabilities.maxImageExtent.height)
			{
				CORE_LOG(HE_WARNING, "VulkanSwapchain", "Requested image size is not supported. Using the current extent size.");
				SetNewImageSize({ surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height });
			}
			else
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested image size is supported");
		}

		// Get the present mode count
		uint32 presentModeCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");
		CORE_ASSERT(presentModeCount > 0, "VulkanSwapchain", "No present modes found");

		// Get the present modes
		ArrayList<VkPresentModeKHR> presentModes(presentModeCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");

		// Check if the requested present mode is supported
		for (auto& mode : presentModes)
		{
			if (mode == VulkanSwapchainUtils::GetVkPresentMode(GetPresentMode()))
			{
				CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Requested present mode is supported");
				break;
			}
		}

		// Get the present queue family info and check if it supports present
		auto pVkQueue = std::static_pointer_cast<VulkanQueue>(GetPresentQueue());
		uint32 presentQueueFamilyIndex = pVkQueue->GetQueueFamilyIndex();
		CORE_ASSERT(presentQueueFamilyIndex != UINT32_MAX, "VulkanSwapchain", "Failed to get present queue family index");
		VkBool32 presentSupport = false;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mVkPhysicalDevice, presentQueueFamilyIndex, mVkSurface, &presentSupport) == VK_SUCCESS,
			"VulkanSwapchain", "Failed to get present support");

		CORE_ASSERT(presentSupport, "VulkanSwapchain", "Present support not found");

		// Create the swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mVkSurface;
		swapchainInfo.minImageCount = GetBufferCount();
		swapchainInfo.imageFormat = VulkanTextureUtils::GetVkTextureFormat(GetSwapchainFormat());
		swapchainInfo.imageColorSpace = colorSpace;
		swapchainInfo.imageExtent = { GetImageSize().x, GetImageSize().y };
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VulkanTextureUtils::GetVkTextureUsageFlags(GetSwapchainUsage());
		swapchainInfo.imageSharingMode = VulkanCoreUtils::GetVkSharingMode(GetShareMode());
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &presentQueueFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = VulkanSwapchainUtils::GetVkPresentMode(GetPresentMode());
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateSwapchainKHR(mVkDevice, &swapchainInfo, nullptr, &mVkSwapchain) == VK_SUCCESS, "VulkanSwapchain", "Failed to create swapchain");

		// After creating the swapchain, get the images from the swapchain
		// Create the images without initializing them since we already have
		// VkImage from the swapchain

		uint32 imageCount = 0;
		CORE_ASSERT(vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchain, &imageCount, nullptr) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");
		CORE_ASSERT(imageCount > 0, "VulkanSwapchain", "No swapchain images found");

		ArrayList<VkImage> images(imageCount);
		CORE_ASSERT(vkGetSwapchainImagesKHR(mVkDevice, mVkSwapchain, &imageCount, images.data()) == VK_SUCCESS, "VulkanSwapchain", "Failed to get swapchain images");

		// Nevertheless, we need to fill the VulkanTexture data
		for (uint32 i = 0; i < imageCount; i++)
		{
			TextureDesc textureDesc = {};
			textureDesc.ArraySize = 1;
			textureDesc.ImageFormat = GetSwapchainFormat();
			textureDesc.ImageSize = { GetImageSize().x, GetImageSize().y, 1 };
			textureDesc.MipLevels = 1;
			textureDesc.SampleCount = TextureSampleCount::Sample1;
			textureDesc.Type = TextureType::Texture2D;
			textureDesc.Usage = GetSwapchainUsage();

			auto pTexture = device->CreateTextureForSwapchain(textureDesc, images[i]);
			AddTexture(pTexture);

			// After creating the VulkanTexture, we need to create the VulkanTextureView
			TextureBufferDesc viewDesc = {};
			viewDesc.pTexture = pTexture;
			viewDesc.ArrayLayer = 0;
			viewDesc.MipLevel = 0;
			viewDesc.AspectFlags = TextureAspectFlags::ColorAspect;

			auto pTextureView = device->CreateTextureBuffer(viewDesc);
			AddTextureBuffer(pTextureView);
		}
	}

	void VulkanSwapchain::PresentImpl(SharedPtr<Semaphore> ppSignalSemaphores[], uint32 amount)
	{
		CORE_ASSERT(amount < 16, "VulkanSwapchain", "Too many semaphores");

		// Acquire the next image
		uint32 imageIndex = GetCurrentFrameIndex();

		auto imageSemaphore = std::static_pointer_cast<VulkanSemaphore>(GetImageSemaphore(imageIndex));

		CORE_ASSERT(vkAcquireNextImageKHR(mVkDevice, mVkSwapchain, uint64_max, imageSemaphore->GetVkSemaphore(), VK_NULL_HANDLE, &imageIndex) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to acquire next image");

		VkSemaphore signalSemaphores[16] = {};
		for (byte i = 0; i < amount; i++)
		{
			auto semaphore = std::static_pointer_cast<VulkanSemaphore>(ppSignalSemaphores[i]);
			signalSemaphores[i] = semaphore->GetVkSemaphore();
		}

		PipelineStageFlags flags = PipelineStageFlags::ColorAttachmentOutput;
		VkSemaphore waitSemaphores[1] = { imageSemaphore->GetVkSemaphore() };

		auto pWaitSemaphore = GetImageSemaphore(imageIndex);
		
		GetOwnerDevice()->SubmitToQueue(GetPresentQueue(), nullptr, 0, &pWaitSemaphore, 1, &flags, ppSignalSemaphores, amount, nullptr);

		// Present the image
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = amount;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mVkSwapchain;
		presentInfo.pImageIndices = &imageIndex;

		auto pQueue = std::static_pointer_cast<VulkanQueue>(GetPresentQueue());
		CORE_ASSERT(vkQueuePresentKHR(pQueue->GetVkQueue(), &presentInfo) == VK_SUCCESS, "VulkanSwapchain", "Failed to present image");
	}
}
