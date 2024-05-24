#include "VulkanInstance.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <Runtime/Vulkan/Device/VulkanDevice.h>

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
			CORE_LOG(HE_VERBOSE, "Vulkan", "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			CORE_LOG(HE_INFO, "Vulkan", "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			CORE_LOG(HE_WARNING, "Vulkan", "%s", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			CORE_LOG(HE_ERROR, "Vulkan", "%s", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
#endif

	VulkanInstance::VulkanInstance(const GraphicsInstanceDesc& desc) : GraphicsInstance(desc), mVkInstance(VK_NULL_HANDLE), mInstanceLayers()
	{
#if defined(HOLLOW_DEBUG)
		mVkDebugMessenger = VK_NULL_HANDLE;
#endif

		// Sub-struct to control the supported extensions
		struct ExtensionEntry
		{
			const char* pExtensionName;
			bool bSupported;
		};

		// First request the extensions we want as false.
		ArrayList<ExtensionEntry> requestedExtensions = {};
		requestedExtensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME , false });
		requestedExtensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });

#if defined(HOLLOW_PLATFORM_WINDOWS)
		requestedExtensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif

#if defined(HOLLOW_DEBUG)
		requestedExtensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		requestedExtensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif

		// Check for instance extension support
		uint32 extensionCount = 0;
		CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance extension properties");
		CORE_ASSERT(extensionCount > 0, "VulkanInstance", "No instance extension properties found");

		// Get the supported extensions
		ArrayList<VkExtensionProperties> supportedExtensions(extensionCount);
		CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance extension properties");

#if defined(HOLLOW_DEBUG)
		for (auto& extension : supportedExtensions)
			CORE_LOG(HE_INFO, "DEBUG INFO SUPPORTED EXTENSIONS", "%s", extension.extensionName);
#endif

		// Check if the requested extensions are supported by the instance
		for (byte index = 0; index < static_cast<byte>(requestedExtensions.size()); index++)
		{
			for (auto& extension : supportedExtensions)
			{
				if (strcmp(requestedExtensions[index].pExtensionName, extension.extensionName) == 0)
				{
					requestedExtensions[index].bSupported = true;
					break;
				}
			}
		}

		// Seperate the supported and unsupported extensions
		ArrayList<const char*> supportedExtensionNames = {};
		ArrayList<const char*> unsupportedExtensionNames = {};

		for (auto& extension : requestedExtensions)
		{
			if (extension.bSupported)
				supportedExtensionNames.push_back(extension.pExtensionName);
			else
				unsupportedExtensionNames.push_back(extension.pExtensionName);
		}

		for (auto& extension : unsupportedExtensionNames)
			CORE_LOG(HE_WARNING, "VulkanInstance", "Unsupported extension: %s", extension);

		// Get supported layers
		uint32 supportedLayerCount = 0;
		CORE_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance layer properties");
		CORE_ASSERT(supportedLayerCount > 0, "VulkanInstance", "No instance layer properties found");

		ArrayList<VkLayerProperties> supportedLayers(supportedLayerCount);
		CORE_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance layer properties");

#if defined(HOLLOW_DEBUG)
		for (auto& layer : supportedLayers)
			CORE_LOG(HE_INFO, "DEBUG INFO SUPPORTED LAYERS", " % s", layer.layerName);
#endif

		// Add needed layers
		ArrayList<const char*> layerNames = {};
#if defined(HOLLOW_DEBUG)
		layerNames.push_back("VK_LAYER_KHRONOS_validation");
		layerNames.push_back("VK_LAYER_LUNARG_screenshot");
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = desc.ApplicationName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
		appInfo.pEngineName = desc.InstanceName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(1, 3, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		// Instance info
		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = static_cast<uint32>(layerNames.size());
		instanceInfo.ppEnabledLayerNames = layerNames.data();
		instanceInfo.enabledExtensionCount = static_cast<uint32>(supportedExtensionNames.size());
		instanceInfo.ppEnabledExtensionNames = supportedExtensionNames.data();
		instanceInfo.flags = VkInstanceCreateFlags();
		instanceInfo.pNext = nullptr;

		// Create the instance
		CORE_ASSERT(vkCreateInstance(&instanceInfo, nullptr, &mVkInstance) == VK_SUCCESS, "VulkanInstance", "Failed to create instance");

		// Handle debug messenger
#if defined(HOLLOW_DEBUG)

		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT"));

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = nullptr;

		CORE_ASSERT(debugMessengerCreateProc(mVkInstance, &debugMessengerInfo, nullptr, &mVkDebugMessenger) == VK_SUCCESS, "VulkanInstance", "Failed to create debug messenger");
#endif
	}

	void VulkanInstance::OnShutdown()
	{
#if defined(HOLLOW_DEBUG)
		debugMessengerDestroyProc(mVkInstance, mVkDebugMessenger, nullptr);
		mVkDebugMessenger = VK_NULL_HANDLE;
#endif
		if (mVkInstance != VK_NULL_HANDLE)
			vkDestroyInstance(mVkInstance, nullptr);

		CORE_LOG(HE_INFO, "VulkanInstance", "Instance destroyed");
	}

	void VulkanInstance::EnumerateAdaptersImpl()
	{
		// Get the physical devices count
		uint32 deviceCount = 0;
		CORE_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr) == VK_SUCCESS, "VulkanInstance", "Failed to enumerate physical devices");
		CORE_ASSERT(deviceCount > 0, "VulkanInstance", "No physical devices found");

		// Get the physical devices
		ArrayList<VkPhysicalDevice> devices(deviceCount);
		CORE_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data()) == VK_SUCCESS, "VulkanInstance", "Failed to enumerate physical devices");

		// Fill the adapter list
		for (auto device : devices)
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
			uint32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
			CORE_ASSERT(queueFamilyCount > 0, "VulkanInstance", "No queue family properties found");

			ArrayList<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

			// Fill the adapter
			SharedPtr<GraphicsAdapter> adapterDesc = std::make_shared<GraphicsAdapter>();
			adapterDesc->ProductName = deviceProperties.deviceName;
			adapterDesc->DeviceId = deviceProperties.deviceID;
			adapterDesc->AdapterScore = deviceProperties.limits.maxImageDimension1D + deviceProperties.limits.maxImageDimension2D + deviceProperties.limits.maxImageDimension3D;
			adapterDesc->VRam = deviceMemoryProperties.memoryHeaps[0].size;

			switch (deviceProperties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				adapterDesc->AdapterType = GraphicsAdapterType::Other;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				adapterDesc->AdapterType = GraphicsAdapterType::Integrated;
				adapterDesc->AdapterScore += 100;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				adapterDesc->AdapterType = GraphicsAdapterType::Discrete;
				adapterDesc->AdapterScore += 1000;
				break;
			default:
				CORE_LOG(HE_ERROR, "VulkanInstance", "Unknown adapter type");
				break;
			}

			if (deviceFeatures.geometryShader)
				adapterDesc->AdapterScore += 100;

			if (deviceFeatures.tessellationShader)
				adapterDesc->AdapterScore += 100;

			CORE_LOG(HE_WARNING, "Adapter Device Name", "%s", adapterDesc->ProductName.c_str());
			CORE_LOG(HE_INFO, "Device ID", "%d ", adapterDesc->DeviceId);
			CORE_LOG(HE_VERBOSE, "Total Score", "%d", adapterDesc->AdapterScore);

			AddAdapter(adapterDesc);
		}
	}
}
