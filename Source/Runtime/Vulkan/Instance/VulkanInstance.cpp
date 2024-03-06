#include "VulkanInstance.h"

#ifdef HOLLOW_PLATFORM_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <Windows.h>
#include <Vulkan/Include/vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Vulkan/Instance/VulkanAdapter.h>

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

	Hollow::VulkanInstance::VulkanInstance(const GraphicsInstanceDesc& desc) : GraphicsInstance(desc), mVkInstance(VK_NULL_HANDLE),
		mVkDebugMessenger(VK_NULL_HANDLE)
	{
		// Sub-struct to control the supported extensions
		struct ExtensionEntry
		{
			const char* pExtensionName;
			bool bSupported;
		};

		// First request the extensions we want as false.
		Array<ExtensionEntry> requestedExtensions = {};
		requestedExtensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });
		requestedExtensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME , false });

		// Platform specific extensions
#ifdef HOLLOW_PLATFORM_WINDOWS
		requestedExtensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif
		// Debug extensions
#ifdef HOLLOW_DEBUG
		requestedExtensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		requestedExtensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif

		// Check for instance extension support
		uint32 extensionCount = 0;
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance extension properties");
		DEV_ASSERT(extensionCount > 0, "VulkanInstance", "No instance extension properties found");

		// Get the supported extensions
		Array<VkExtensionProperties> supportedExtensions(extensionCount);
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance extension properties");

#ifdef HOLLOW_DEBUG
		for (auto& extension : supportedExtensions)
		{
			CORE_LOG(HE_INFO, "DEBUG INFO SUPPORTED EXTENSIONS", "%s", extension.extensionName);
		}
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
		Array<const char*> supportedExtensionNames = {};
		Array<const char*> unsupportedExtensionNames = {};

		for (auto& extension : requestedExtensions)
		{
			if (extension.bSupported)
			{
				supportedExtensionNames.push_back(extension.pExtensionName);
			}
			else
			{
				unsupportedExtensionNames.push_back(extension.pExtensionName);
			}
		}

		// Get supported layers
		uint32 supportedLayerCount = 0;
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance layer properties");
		DEV_ASSERT(supportedLayerCount > 0, "VulkanInstance", "No instance layer properties found");

		Array<VkLayerProperties> supportedLayers(supportedLayerCount);
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()) == VK_SUCCESS, "VulkanInstance",
			"Failed to enumerate instance layer properties");

#ifdef HOLLOW_DEBUG
		for (auto& layer : supportedLayers)
		{
			CORE_LOG(HE_INFO, "DEBUG INFO SUPPORTED LAYERS", " % s", layer.layerName);
		}
#endif

		// Add needed layers
		Array<const char*> layerNames = {};
		layerNames.push_back("VK_LAYER_KHRONOS_validation");

		// Application info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = desc.ApplicationName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = desc.InstanceName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

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
		DEV_ASSERT(vkCreateInstance(&instanceInfo, nullptr, &mVkInstance) == VK_SUCCESS, "CreateInstance", "Failed to create instance");

		// Handle debug messenger
#ifdef HOLLOW_DEBUG

		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT"));

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = nullptr;

		DEV_ASSERT(debugMessengerCreateProc(mVkInstance, &debugMessengerInfo, nullptr, &mVkDebugMessenger) == VK_SUCCESS, "CreateInstance", "Failed to create debug messenger");
#endif
	}

	void VulkanInstance::OnShutdown()
	{
		if (mVkInstance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(mVkInstance, nullptr);
			mVkInstance = VK_NULL_HANDLE;
		}

#ifdef HOLLOW_DEBUG
		debugMessengerDestroyProc(mVkInstance, mVkDebugMessenger, nullptr);
		mVkDebugMessenger = VK_NULL_HANDLE;
#endif
	}

	void VulkanInstance::EnumerateAdaptersImpl()
	{
		// Get the physical devices count
		uint32 deviceCount = 0;
		DEV_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr) == VK_SUCCESS, "VulkanInstance", "Failed to enumerate physical devices");
		DEV_ASSERT(deviceCount > 0, "VulkanInstance", "No physical devices found");

		// Get the physical devices
		Array<VkPhysicalDevice> devices(deviceCount);
		DEV_ASSERT(vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data()) == VK_SUCCESS, "VulkanInstance", "Failed to enumerate physical devices");

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
			DEV_ASSERT(queueFamilyCount > 0, "VulkanInstance", "No queue family properties found");

			Array<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

			// Fill the adapter
			VulkanAdapter adapterDesc = {};
			adapterDesc.ProductName = deviceProperties.deviceName;
			adapterDesc.DeviceId = deviceProperties.deviceID;
			adapterDesc.AdapterScore = deviceProperties.limits.maxImageDimension1D + deviceProperties.limits.maxImageDimension2D + deviceProperties.limits.maxImageDimension3D;
			adapterDesc.VRam = deviceMemoryProperties.memoryHeaps[0].size;
			adapterDesc.PhysicalDevice = device;

			CORE_LOG(HE_WARNING, "GraphicsInstance", "Adapter Device Name: %s", adapterDesc.ProductName.c_str());

			switch (deviceProperties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				adapterDesc.AdapterType = GraphicsAdapterType::Other;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				adapterDesc.AdapterType = GraphicsAdapterType::Integrated;
				adapterDesc.AdapterScore += 100;
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				adapterDesc.AdapterType = GraphicsAdapterType::Discrete;
				adapterDesc.AdapterScore += 1000;
				break;
			default:
				CORE_LOG(HE_FATAL, "GraphicsInstance", "Unknown adapter type");
				break;
			}

			if(deviceFeatures.geometryShader)
			{
				adapterDesc.AdapterScore += 100;
			}
		}
	}

	SharedPtr<GraphicsDevice> VulkanInstance::CreateDeviceImpl()
	{
		GraphicsDeviceDesc desc = {};
		desc.API = GraphicsAPI::Vulkan;
		desc.Instance = this;
		desc.GraphicsQueueCount = 1;
		desc.ComputeQueueCount = 1;
		desc.TransferQueueCount = 1;

		return nullptr; //std::make_shared<VulkanDevice>(desc);
	}
}