#include "VInstance.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif // !NOMINMAX

#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define EXTENSION_SIZE 5

namespace Hollow
{
#if defined(HOLLOW_DEBUG)
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
			CORE_LOG(HE_VERBOSE, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			CORE_LOG(HE_INFO, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			CORE_LOG(HE_WARNING, "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			CORE_LOG(HE_ERROR, "%s", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
#endif

	VInstance::VInstance(const GraphicsInstanceDesc& desc) : GraphicsInstance(desc), mVkAdapter(VK_NULL_HANDLE), mVkInstance(VK_NULL_HANDLE)
	{
#if defined(HOLLOW_DEBUG)
		mVkDebugger = VK_NULL_HANDLE;
#endif
		// Define the wanted extensions
		struct ExtensionEntry
		{
			const char* ExtensionName;
			bool bSupported;
		};

		// Fill the wanted extensions
		DArray<ExtensionEntry> extensions(EXTENSION_SIZE);
		extensions[0] = { VK_KHR_SURFACE_EXTENSION_NAME, false };
		extensions[1] = { VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false };

#if defined(HOLLOW_PLATFORM_WINDOWS)
		extensions[2] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false };
#endif // HOLLOW_PLATFORM_WINDOWS
#if defined(HOLLOW_DEBUG)
		extensions[3] = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false };
		extensions[4] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false };
#endif // HOLLOW_DEBUG

		// Check the total number of available extensions for the current computer
		u32 extensionCount = 0;
		CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) == VK_SUCCESS,
			"VInstance", "Failed to enumerate instance extension properties");
		CORE_ASSERT(extensionCount > 0, "VInstance", "No instance extension properties found");

		// Get all the available extensions for the current computer
		DArray<VkExtensionProperties> allExtensions(extensionCount);
		CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allExtensions.data()) == VK_SUCCESS,
			"VInstance", "Failed to enumerate instance extension properties");

		// Check if the wanted extensions are supported
		for (u8 i = 0; i < EXTENSION_SIZE; ++i)
		{
			for (auto& extension : allExtensions)
			{
				if (strcmp(extensions[i].ExtensionName, extension.extensionName) == 0)
				{
					extensions[i].bSupported = true;
					break;
				}
			}
		}

		// Seperate the names of working and not working extensions
		DArray<const char*> workingExtensions;
		DArray<const char*> brokenExtensions;

		for (auto& extension : extensions)
		{
			if (extension.bSupported)
				workingExtensions.push_back(extension.ExtensionName);
			else
				brokenExtensions.push_back(extension.ExtensionName);
		}

		// Check the total number of available layers for the current computer
		u32 layerCount = 0;
		CORE_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, nullptr) == VK_SUCCESS,
			"VInstance", "Failed to enumerate instance layer properties");
		CORE_ASSERT(layerCount > 0, "VInstance", "No instance layer properties found");

		// Get all the available layers for the current computer
		DArray<VkLayerProperties> allLayers(layerCount);
		CORE_ASSERT(vkEnumerateInstanceLayerProperties(&layerCount, allLayers.data()) == VK_SUCCESS,
			"VInstance", "Failed to enumerate instance layer properties");

		// Get all the wanted layers for debugging
		DArray<const char*> layers;
#if defined(HOLLOW_DEBUG)
		layers.push_back("VK_LAYER_KHRONOS_validation");
		layers.push_back("VK_LAYER_LUNARG_screenshot");
		layers.push_back("VK_LAYER_LUNARG_monitor");
#endif // HOLLOW_DEBUG

		// Create the application info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = desc.AppName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(desc.AppVersion.x, desc.AppVersion.y, desc.AppVersion.z);
		appInfo.pEngineName = desc.EngineName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(desc.EngineVersion.x, desc.EngineVersion.y, desc.EngineVersion.z);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Create the instance info
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = (u32)workingExtensions.size();
		createInfo.ppEnabledExtensionNames = workingExtensions.data();
		createInfo.enabledLayerCount = (u32)layers.size();
		createInfo.ppEnabledLayerNames = layers.data();
		createInfo.flags = 0;

		CORE_ASSERT(vkCreateInstance(&createInfo, nullptr, &mVkInstance) == VK_SUCCESS, "VInstance", "Failed to create Vulkan instance");

#if defined(HOLLOW_DEBUG)
		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT"));

		// Create the debug messenger for debugging
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = nullptr;

		CORE_ASSERT(debugMessengerCreateProc(mVkInstance, &debugMessengerInfo, nullptr, &mVkDebugger) == VK_SUCCESS, "VInstance", "Failed to create debug messenger");
#endif

		// Get the best adapter for the current computer
		mVkAdapter = GetBestAdapter();
	}

	VkPhysicalDevice VInstance::GetBestAdapter()
	{
		// Get the physical devices count
		u32 deviceCount = 0;
		CORE_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr) == VK_SUCCESS, "VInstance", "Failed to enumerate physical devices");
		CORE_ASSERT(deviceCount > 0, "VInstance", "No physical devices found");

		// Temporary struct to hold the device and its score
		Map<String, Pair<VkPhysicalDevice, u32>> allDevices;

		// Get the physical devices
		DArray<VkPhysicalDevice> devices(deviceCount);
		CORE_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data()) == VK_SUCCESS, "VInstance", "Failed to enumerate physical devices");

		for (auto& device : devices)
		{
			// Get the device properties
			VkPhysicalDeviceProperties deviceProperties = {};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			// Get the device features
			VkPhysicalDeviceFeatures deviceFeatures = {};
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			// Get the device memory properties
			VkPhysicalDeviceMemoryProperties deviceMemoryProperties = {};
			vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

			// Get the device queue family properties
			u32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			CORE_ASSERT(queueFamilyCount > 0, "VInstance", "No queue family properties found");

			DArray<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

			// Hold device on allDevices
			allDevices[deviceProperties.deviceName] = { device, 0 };

			// Check if the device is discrete
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				allDevices[deviceProperties.deviceName].second += 1000;
			else
				allDevices[deviceProperties.deviceName].second += 100;

			// Check if the device supports geometry shaders
			if (deviceFeatures.geometryShader)
				allDevices[deviceProperties.deviceName].second += 10;
		}

		// Get the best device
		auto bestDevice = std::max_element(allDevices.begin(), allDevices.end(),
			[](const Pair<String, Pair<VkPhysicalDevice, u32>>& a, const Pair<String, Pair<VkPhysicalDevice, u32>>& b)
			{
				return a.second.second < b.second.second;
			});

		CORE_LOG(HE_INFO, "Best device found: %s", bestDevice->first.c_str());
		return bestDevice->second.first;
	}
}
