#include "VulkanInstance.h"

#include <Runtime/Vulkan/Adapter/VulkanAdapter.h>

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
		struct ExtensionEntry
		{
			const char* pLayerName;
			bool bSupported;
		};

		// Create instance extensions
		Array<ExtensionEntry> instanceRequestedExtensions;
		for (const String extension : desc.EnabledExtensions)
		{
			instanceRequestedExtensions.push_back({ extension.c_str(), false });
		}

		// Get supported instance extensions count
		uint32 supportedInstanceExtensionCount = 0;
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &supportedInstanceExtensionCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to get instance extension count");
		DEV_ASSERT(supportedInstanceExtensionCount > 0, "VulkanInstance", "No instance extensions found");

		// Get supported instance extensions
		Array<VkExtensionProperties> supportedExtensions(supportedInstanceExtensionCount);
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &supportedInstanceExtensionCount, supportedExtensions.data()) == VK_SUCCESS, "VulkanInstance",
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

		// Check if extension names are supported
		for (byte index = 0; index < static_cast<byte>(instanceRequestedExtensions.size()); index++)
		{
			ExtensionEntry& entry = instanceRequestedExtensions[index];

			// Check if props contains the requested extension
			bool bExtensionSupported = false;
			for (byte i = 0; i < static_cast<byte>(supportedInstanceExtensionCount); i++)
			{
				if (strcmp(supportedExtensions[i].extensionName, entry.pLayerName) == 0)
				{
					bExtensionSupported = true;
					break;
				}
			}

			entry.bSupported = bExtensionSupported;
		}

		// Filter out unsupported extensions
		Array<const char*> selectedExtensions;
		Array<const char*> unsupportedExtensions;
		for (byte i = 0; i < static_cast<byte>(instanceRequestedExtensions.size()); i++)
		{
			if (instanceRequestedExtensions[i].bSupported)
			{
				selectedExtensions.push_back(instanceRequestedExtensions[i].pLayerName);
			}
			else
			{
				unsupportedExtensions.push_back(instanceRequestedExtensions[i].pLayerName);
			}
		}

		// Get supported layers count
		uint32 supportedLayerCount = 0;
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to get instance layer count");
		DEV_ASSERT(supportedLayerCount > 0, "VulkanInstance", "No instance layers found");

		Array<VkLayerProperties> supportedLayers(supportedLayerCount);
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to get instance layer count");

#ifdef HOLLOW_DEBUG
		for (const VkLayerProperties& layer : supportedLayers)
			CORE_LOG(HE_INFO, "VulkanInstance", "Supported layer: %s", layer.layerName);
#endif

		// Get supported instance layers
		Array<const char*> selectedLayers = {};
#ifdef HOLLOW_DEBUG
		selectedLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif 

		// Add disabled extensions
		VkValidationFeaturesEXT validationFeatures = {};
		validationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		validationFeatures.disabledValidationFeatureCount = 1;
		VkValidationFeatureDisableEXT disabledFeatures[] = {
			VK_VALIDATION_FEATURE_DISABLE_SHADER_VALIDATION_CACHE_EXT,
		};
		validationFeatures.pDisabledValidationFeatures = disabledFeatures;

		// Create instance info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Runtime";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Hollow Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instanceCreateInfo = { };
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32>(selectedExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = selectedExtensions.data();
		instanceCreateInfo.enabledLayerCount = static_cast<uint32>(selectedLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = selectedLayers.data();
		instanceCreateInfo.pNext = &validationFeatures;

		DEV_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &mVkInstance) == VK_SUCCESS, "VulkanInstance", "Failed to create instance");

#ifdef HOLLOW_DEBUG
		// Get debug messenger proc
		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT"));

		DEV_ASSERT(debugMessengerCreateProc != nullptr, "VulkanInstance", "Failed to get debug messenger create proc");
		DEV_ASSERT(debugMessengerDestroyProc != nullptr, "VulkanInstance", "Failed to get debug messenger destroy proc");

		// Create debug messenger
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		DEV_ASSERT(debugMessengerCreateProc(mVkInstance, &debugCreateInfo, nullptr, &mDebugMessenger) == VK_SUCCESS, "VulkanInstance",
			"Failed to create debug messenger");

		for (const char* extension : unsupportedExtensions)
			CORE_LOG(HE_WARNING, "VulkanInstance", "Unsupported extension: %s", extension);
		for (const char* layer : selectedLayers)
			CORE_LOG(HE_INFO, "VulkanInstance", "Selected layer: %s", layer);
		for (const char* extension : selectedExtensions)
			CORE_LOG(HE_INFO, "VulkanInstance", "Selected extension: %s", extension);
#endif

		// Get physical devices
		uint32 physicalDeviceCount = 0;
		DEV_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &physicalDeviceCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to get physical device count");
		DEV_ASSERT(physicalDeviceCount > 0, "VulkanInstance", "No physical devices found");

		Array<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		DEV_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &physicalDeviceCount, physicalDevices.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to get physical devices");

		for (const VkPhysicalDevice& device : physicalDevices)
		{
			VkPhysicalDeviceMemoryProperties memoryProperties = {};
			vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

			VkPhysicalDeviceProperties deviceProperties = {};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			VkPhysicalDeviceFeatures deviceFeatures = {};
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			GraphicsAdapterDesc adapterDesc = {};
			adapterDesc.ProductName = deviceProperties.deviceName;
			adapterDesc.pInstance = this;
			VkDeviceSize totalVRam = 0;

			for (uint32 i = 0; i < memoryProperties.memoryHeapCount; i++)
			{
				if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				{
					totalVRam += memoryProperties.memoryHeaps[i].size;
				}
			}

			adapterDesc.VRam = totalVRam;

			SharedPtr<VulkanAdapter> adapter = std::make_shared<VulkanAdapter>(adapterDesc, device);
			AddAdapter(adapter);
		}
	}

	void VulkanInstance::OnShutdown()
	{
#ifdef HOLLOW_DEBUG
		debugMessengerDestroyProc(mVkInstance, mDebugMessenger, nullptr);
#endif

		vkDestroyInstance(mVkInstance, nullptr);
		CORE_LOG(HE_WARNING, "Vulkan Instance", "Shutting down...");
	}
}
