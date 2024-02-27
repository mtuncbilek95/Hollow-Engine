#include "VulkanSwapchain.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Window/WindowManager.h>

#ifdef HOLLOW_PLATFORM_WINDOWS
#include <vulkan_win32.h>
#endif

#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchainUtils.h>


#include <algorithm>

namespace Hollow
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice) : Swapchain(desc, pDevice)
	{
		mVkSurface = VK_NULL_HANDLE;
		mVkSwapchain = VK_NULL_HANDLE;

		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
		mVkPhysicalDevice = pDevice->GetVkPhysicalDevice();
		mVkInstance = static_cast<VulkanInstance*>(pDevice->GetGraphicsAdapter()->GetInstance())->GetVkInstance();

		ResizeCore(WindowManager::GetInstance().GetMainWindow()->GetWindowSize());
	}

	void VulkanSwapchain::Free()
	{
		if (mVkSwapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(mVkLogicalDevice, mVkSwapchain, nullptr);
			mVkSwapchain = VK_NULL_HANDLE;
		}

		if (mVkSurface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(mVkInstance, mVkSurface, nullptr);
			mVkSurface = VK_NULL_HANDLE;
		}
	}

	void VulkanSwapchain::OnShutdown() noexcept
	{
		Free();
	}

	void VulkanSwapchain::PresentCore(Semaphore** ppSemaphores, uint32 waitSemaphoreCount)
	{
		VkFence fence = static_cast<VulkanFence*>(GetFence(GetCurrentFrameIndex()).get())->GetVkFence();

		// Acquire the next image
		uint32 imageIndex = 0;
		DEV_ASSERT(vkAcquireNextImageKHR(mVkLogicalDevice, mVkSwapchain, UINT64_MAX, VK_NULL_HANDLE, fence, &imageIndex) == VK_SUCCESS, "VulkanSwapchain", "Failed to acquire next image.");

		// Wait for the fence
		VkSemaphore pSemaphores[32];
		for (uint32 i = 0; i < waitSemaphoreCount; i++)
		{
			pSemaphores[i] = static_cast<VulkanSemaphore*>(ppSemaphores[i])->GetVkSemaphore();
		}

		// Create PresentInfo
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = waitSemaphoreCount;
		presentInfo.pWaitSemaphores = pSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mVkSwapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;
		presentInfo.pNext = nullptr;

		// Present the image
		VulkanQueue* vkQ = static_cast<VulkanQueue*>(GetQueue());
		DEV_ASSERT(vkQueuePresentKHR(vkQ->GetVkQueue(), &presentInfo) == VK_SUCCESS, "VulkanSwapchain", "Failed to present image.");
	}

	void VulkanSwapchain::ResizeCore(Vector2u newSize)
	{
		// Free swapchain and surface first.
		Free();

#ifdef HOLLOW_PLATFORM_WINDOWS
		// Create surface for Win32
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd = WindowManager::GetInstance().GetMainWindow()->GetWindowNativeHandle();
		surfaceCreateInfo.flags = VkWin32SurfaceCreateFlagsKHR();

		DEV_ASSERT(vkCreateWin32SurfaceKHR(mVkInstance, &surfaceCreateInfo, nullptr, &mVkSurface) == VK_SUCCESS, "VulkanSwapchain", "Failed to create Win32 surface.");

		CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Win32 surface created.");
#endif

		// Get the surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVkPhysicalDevice, mVkSurface, &surfaceCapabilities) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface capabilities.");

		// Check desc for swapchain settings
		DEV_ASSERT(surfaceCapabilities.maxImageCount >= GetBufferCount(), "VulkanSwapchain", "Surface is supporting %d ");

		// Get the supported surface formats
		uint32 formatCount;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, nullptr) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats count.");

		DEV_ASSERT(formatCount > 0, "VulkanSwapchain", "No surface formats found.");

		Array<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mVkSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get surface formats.");

		// Get the supported present modes
		uint32 presentModeCount;

		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, nullptr) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get present mode count.");
		DEV_ASSERT(presentModeCount > 0, "VulkanSwapchain", "No present modes found.");

		Array<VkPresentModeKHR> presentModes(presentModeCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice, mVkSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get present modes.");

		// Check if chosen present mode is supported
		VkPresentModeKHR presentMode = {};
		bool bRequiredPresentModeSupported = false;

		for (const auto& mode : presentModes)
		{
			if (mode == VulkanSwapchainUtils::GetVkPresentMode(GetPresentMode()))
			{
				presentMode = mode;
				bRequiredPresentModeSupported = true;
				break;
			}
		}

		// Select the best surface format
		VkSurfaceFormatKHR surfaceFormat = surfaceFormats[0];

		VkFormat requestedFormat = VulkanTextureUtils::GetVkTextureFormat(GetSwapchainFormat());

		bool bRequiredFormatSupported = false;
		for (const auto& format : surfaceFormats)
		{
			if (format.format == requestedFormat && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				surfaceFormat = format;
				bRequiredFormatSupported = true;
				break;
			}
		}

		DEV_ASSERT(bRequiredFormatSupported, "VulkanSwapchain", "Required format is not supported.");

		// Select the swap extent
		VkExtent2D swapExtent =
		{
			std::clamp(newSize.x, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
			std::clamp(newSize.y, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
		};

		// Select the number of images
		uint32 imageCount = GetBufferCount() > surfaceCapabilities.maxImageCount ? surfaceCapabilities.maxImageCount : GetBufferCount();

		//Get present family index
		VulkanQueue* vkQ = static_cast<VulkanQueue*>(GetQueue());
		uint32 presentFamilyIndex = vkQ->GetQueueFamilyIndex();
		VkBool32 presentSupport = false;

		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mVkPhysicalDevice, presentFamilyIndex, mVkSurface, &presentSupport) == VK_SUCCESS, 
			"VulkanSwapchain", "Failed to get present support.");
		DEV_ASSERT(presentSupport, "VulkanSwapchain", "Present support is not available.");

		// Create the swapchain
		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = mVkSurface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = swapExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;

		// It should be set to VK_TRUE, if application doesn't want to read from swap chain images 
		swapchainCreateInfo.clipped = VK_FALSE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		swapchainCreateInfo.pQueueFamilyIndices = &presentFamilyIndex;
		swapchainCreateInfo.queueFamilyIndexCount = 1;

		swapchainCreateInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateSwapchainKHR(mVkLogicalDevice, &swapchainCreateInfo, nullptr, &mVkSwapchain) == VK_SUCCESS,
			"VulkanSwapchain", "Failed to create swapchain.");

		CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Swapchain created.");

		// Get the swapchain image count
		uint32 swapchainImageCount;
		DEV_ASSERT(vkGetSwapchainImagesKHR(mVkLogicalDevice, mVkSwapchain, &swapchainImageCount, nullptr) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get swapchain image count.");

		Array<SharedPtr<Texture>> swapchainImages(swapchainImageCount);
		Array<VkImage> vkSwapchainImages(swapchainImageCount);

		// Get the swapchain images
		DEV_ASSERT(vkGetSwapchainImagesKHR(mVkLogicalDevice, mVkSwapchain, &swapchainImageCount, vkSwapchainImages.data()) == VK_SUCCESS, "VulkanSwapchain",
			"Failed to get swapchain images.");

		// Create the swapchain texture objects
		for (uint32 i = 0; i < swapchainImageCount; i++)
		{
			TextureDesc desc;
			desc.ArraySize = 1;
			desc.Type = TextureType::Texture2D;
			desc.Format = GetSwapchainFormat();
			desc.Usages = { TextureUsage::ColorAttachment };
			desc.ImageSize = { swapExtent.width, swapExtent.height, 1 };
			desc.MipLevels = 1;
			desc.SampleCount = TextureSampleCount::Sample1;
			desc.pMemory = nullptr;

			swapchainImages[i] = reinterpret_cast<VulkanDevice*>(GetOwnerDevice())->CreateVkTextureForSwapchain(desc, vkSwapchainImages[i]);
		}

		// Create the depth stencil image
		// TODO: Implement depth stencil image creation
		
		// Create the color image views
		CreateColorImageViews(swapchainImages);
	}
}
