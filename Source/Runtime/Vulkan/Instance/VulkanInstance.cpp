#include "VulkanInstance.h"

#ifdef HOLLOW_PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan_win32.h>
#endif

namespace Hollow
{
#ifdef HOLLOW_DEBUG
	PFN_vkCreateDebugUtilsMessengerEXT debugMessengerCreateProc = nullptr;
	PFN_vkDestroyDebugUtilsMessengerEXT debugMessengerDestroyProc = nullptr;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			DEV_LOG(HE_VERBOSE, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			DEV_LOG(HE_INFO, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			DEV_LOG(HE_WARNING, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			DEV_LOG(HE_ERROR, "%s", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
#endif

	VulkanInstance::VulkanInstance(const GraphicsInstanceDesc& desc) : GraphicsInstance(desc)
	{
		struct LayerEntry
		{
			const char* pLayerName;
			bool bSupported;
		};

		// Create instance extensions
		Array<LayerEntry> instanceRequestedExtensions;
		for (const String extension : desc.EnabledExtensions)
		{
			instanceRequestedExtensions.push_back({ extension.c_str(), false });
		}

		// Get supported instance extensions
		uint32 instanceExtensionCount = 0;
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to get instance extension count");
		DEV_ASSERT(instanceExtensionCount > 0, "VulkanInstance", "No instance extensions found");

		// Get supported instance layers
		Array<VkLayerProperties> instanceLayers;
		uint32 instanceLayerCount = 0;
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to get instance layer count");

		// Add requested instance extensions
		instanceRequestedExtensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });
		instanceRequestedExtensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, false });


		// If platform is windows, add windows specific extensions
#ifdef HOLLOW_PLATFORM_WINDOWS
		instanceRequestedExtensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif

		// If debug is enabled, add debug extensions
#ifdef HOLLOW_DEBUG
		instanceRequestedExtensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		instanceRequestedExtensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif
	}
}
