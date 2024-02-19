#include "VulkanSwapchain.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Window/WindowManager.h>

#ifdef HOLLOW_PLATFORM_WINDOWS
#include <vulkan_win32.h>
#endif

#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchainUtils.h>

#include <algorithm>

namespace Hollow
{
	VulkanSwapchain::VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice) : Swapchain(desc)
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
	}

	void VulkanSwapchain::PresentCore()
	{
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

		VkFormat requestedFormat = VulkanTextureUtils::GetVkFormat(GetSwapchainFormat());

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
		VulkanQueue* pQueue; // TODO: Get the queue from the device
		uint32 presentFamilyIndex; // TODO: Get the present family index from the queue
		VkBool32 presentSupport = false;

		//vkGetPhysicalDeviceSurfaceSupportKHR(mVkPhysicalDevice, presentFamilyIndex, mVkSurface, &presentSupport);

		//DEV_ASSERT(presentSupport, "VulkanSwapchain", "Present support is not available.");

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
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;

		// It should be set to VK_TRUE, if application doesn't want to read from swap chain images 
		// Found in here: https://shorturl.at/eEL58
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		//swapchainCreateInfo.pQueueFamilyIndices = &presentFamilyIndex;
		//swapchainCreateInfo.queueFamilyIndexCount = 1;

		swapchainCreateInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateSwapchainKHR(mVkLogicalDevice, &swapchainCreateInfo, nullptr, &mVkSwapchain) == VK_SUCCESS,
			"VulkanSwapchain", "Failed to create swapchain.");

		CORE_LOG(HE_VERBOSE, "VulkanSwapchain", "Swapchain created.");

		// Get the swapchain images

	}
}
