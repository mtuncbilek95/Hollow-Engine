#include "Renderer.h"

#include <Shader/Shader.h>
#include <Pipeline/Pipeline.h>
#include <RenderPass/RenderPass.h>
#include <Buffer/Buffer.h>
#include <Queue/Queue.h>

namespace MiniVk
{

#ifdef MINIVK_DEBUG
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

	Renderer::Renderer(const RendererDesc& desc) : mWindowNativeHandle(desc.WindowNativeHandle), mWindowSize(desc.WindowSize), mWindowHandle(desc.WindowHandle),
		mInstance(VK_NULL_HANDLE), mPhysicalDevice(VK_NULL_HANDLE), mDevice(VK_NULL_HANDLE), mSurface(VK_NULL_HANDLE), mSwapchain(VK_NULL_HANDLE),
		mDebugMessenger(VK_NULL_HANDLE), mSwapchainImageFormat(VK_FORMAT_B8G8R8A8_UNORM)
	{
	}

	void Renderer::Initialize()
	{
		CreateInstance();
		CreatePhysicalDevice();
		FindQueueFamilies();
		CreateLogicalDevice();
		CreateSurfaceSwapchain();
		CreateSwapchainImages();

		CreateCommandPool();
		CreateCommandBuffer();

		CreateFence();
		CreateSemaphores();

	}

	void Renderer::Shutdown()
	{
		vkDeviceWaitIdle(mDevice);

		vkFreeCommandBuffers(mDevice, mCommandPool, 1, mCommandBuffers.data());
		vkDestroyCommandPool(mDevice, mCommandPool, nullptr);

		for (auto& view : mSwapchainImageViews)
		{
			vkDestroyImageView(mDevice, view, nullptr);
		}

		vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
		vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
		vkDestroyDevice(mDevice, nullptr);

#ifdef MINIVK_DEBUG
		debugMessengerDestroyProc(mInstance, mDebugMessenger, nullptr);
#endif
		vkDestroyInstance(mInstance, nullptr);
	}

	void Renderer::BindBuffer(Buffer* buffer, void* inData)
	{
		void* outData;
		vkMapMemory(mDevice, buffer->GetBufferMemory(), 0, buffer->GetSizeInBytes(), 0, &outData);
		memcpy(outData, inData, buffer->GetSizeInBytes());
		vkUnmapMemory(mDevice, buffer->GetBufferMemory());
	}

	void Renderer::CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer, uint32 sizeInBytes)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		DEV_ASSERT(vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS, "CopyBuffer", "Failed to begin recording command buffer");

		VkBufferCopy copyRegion = {};
		copyRegion.size = sizeInBytes;
		vkCmdCopyBuffer(commandBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &copyRegion);

		DEV_ASSERT(vkEndCommandBuffer(commandBuffer) == VK_SUCCESS, "CopyBuffer", "Failed to end recording command buffer");

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		DEV_ASSERT(vkQueueSubmit(mGraphicsQueue->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE) == VK_SUCCESS, "CopyBuffer", "Failed to submit queue");
		vkQueueWaitIdle(mGraphicsQueue->GetQueue());

		vkFreeCommandBuffers(mDevice, mCommandPool, 1, &commandBuffer);
	}

	void Renderer::BeginRecording(uint32* imageIndex)
	{
		// wait for fences first
		vkWaitForFences(mDevice, 1, &(mFences[*imageIndex]), VK_TRUE, UINT64_MAX);
		vkResetFences(mDevice, 1, &(mFences[*imageIndex]));

		// Acquire the next image
		DEV_ASSERT(vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, mImageSemaphores[*imageIndex], VK_NULL_HANDLE, imageIndex) == VK_SUCCESS, "BeginRecording", "Failed to acquire next image");

		// Reset the command buffer
		vkResetCommandBuffer(mCommandBuffers[*imageIndex], 0);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		DEV_ASSERT(vkBeginCommandBuffer(mCommandBuffers[*imageIndex], &beginInfo) == VK_SUCCESS, "BeginRecording", "Failed to begin recording command buffer");
	}

	void Renderer::BindRenderPass(Pipeline* pipeline, uint32 imageIndex)
	{
		// Begin render pass
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pipeline->GetRenderPass()->GetRenderPass();
		renderPassInfo.framebuffer = pipeline->GetRenderPass()->GetFramebuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { mWindowSize.x, mWindowSize.y };

		VkClearValue clearColor = { 0.1f, 0.2f, 0.3f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(mCommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Bind pipeline
		vkCmdBindPipeline(mCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipeline());

		// bind viewport
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(mWindowSize.x);
		viewport.height = static_cast<float>(mWindowSize.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(mCommandBuffers[imageIndex], 0, 1, &viewport);

		// bind scissor
		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { mWindowSize.x, mWindowSize.y };
		vkCmdSetScissor(mCommandBuffers[imageIndex], 0, 1, &scissor);
	}

	void Renderer::Draw(Buffer* pBuffer, uint32 vertexSize, uint32 imageIndex)
	{
		VkBuffer vertexbuffers[] = { pBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(mCommandBuffers[imageIndex], 0, 1, vertexbuffers, offsets);

		vkCmdDraw(mCommandBuffers[imageIndex], vertexSize, 1, 0, 0);
	}

	void Renderer::DrawIndexed(Buffer* pVertex, Buffer* pIndex, uint32 vertexSize, uint32 indexSize, uint32 imageIndex)
	{
		VkBuffer vertexbuffers[] = { pVertex->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(mCommandBuffers[imageIndex], 0, 1, vertexbuffers, offsets);
		vkCmdBindIndexBuffer(mCommandBuffers[imageIndex], pIndex->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(mCommandBuffers[imageIndex], indexSize, 1, 0, 0, 0);
	}

	void Renderer::EndRecording(uint32 imageIndex)
	{
		vkCmdEndRenderPass(mCommandBuffers[imageIndex]);
		DEV_ASSERT(vkEndCommandBuffer(mCommandBuffers[imageIndex]) == VK_SUCCESS, "EndRecording", "Failed to end recording command buffer");
	}

	void Renderer::Present(uint32 imageIndex)
	{
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { mImageSemaphores[imageIndex]};
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { mRenderSemaphores[imageIndex]};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		DEV_ASSERT(vkQueueSubmit(mGraphicsQueue->GetQueue(), 1, &submitInfo, mFences[imageIndex]) == VK_SUCCESS, "Present", "Failed to submit queue");

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchains[] = { mSwapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;

		DEV_ASSERT(vkQueuePresentKHR(mGraphicsQueue->GetQueue(), &presentInfo) == VK_SUCCESS, "Present", "Failed to present queue");
	}

	void Renderer::CreateInstance()
	{
		struct ExtensionCheck
		{
			const char* LayerName;
			bool bSupported;
		};

		// First request the extensions we want as false.
		Array<ExtensionCheck> requestedExtensions = {};
		requestedExtensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });
		requestedExtensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME , false });

		// Platform specific extensions
#ifdef MINIVK_PLATFORM_WINDOWS
		requestedExtensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif
		// Debug extensions
#ifdef MINIVK_DEBUG
		requestedExtensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		requestedExtensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif

		// Check for instance extension support
		uint32 extensionCount = 0;
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) == VK_SUCCESS, "CreateInstance",
			"Failed to enumerate instance extension properties");
		DEV_ASSERT(extensionCount > 0, "CreateInstance", "No instance extension properties found");

		// Get the supported extensions
		Array<VkExtensionProperties> supportedExtensions(extensionCount);
		DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data()) == VK_SUCCESS, "CreateInstance",
			"Failed to enumerate instance extension properties");

#ifdef MINIVK_DEBUG
		for (auto& extension : supportedExtensions)
		{
			CORE_LOG(HE_INFO, "Supported Extensions", "%s", extension.extensionName);
		}
#endif
		// Check if the requested extensions are supported by the instance
		for (byte index = 0; index < static_cast<byte>(requestedExtensions.size()); index++)
		{
			for (auto& extension : supportedExtensions)
			{
				if (strcmp(requestedExtensions[index].LayerName, extension.extensionName) == 0)
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
				supportedExtensionNames.push_back(extension.LayerName);
			}
			else
			{
				unsupportedExtensionNames.push_back(extension.LayerName);
			}
		}

		// Get supported layers
		uint32 supportedLayerCount = 0;
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) == VK_SUCCESS, "CreateInstance",
			"Failed to enumerate instance layer properties");
		DEV_ASSERT(supportedLayerCount > 0, "CreateInstance", "No instance layer properties found");

		Array<VkLayerProperties> supportedLayers(supportedLayerCount);
		DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()) == VK_SUCCESS, "CreateInstance",
			"Failed to enumerate instance layer properties");

#ifdef MINIVK_DEBUG
		for (auto& layer : supportedLayers)
		{
			CORE_LOG(HE_INFO, "Supported Layers", "%s", layer.layerName);
		}
#endif

		// Add needed layers
		Array<const char*> layerNames = {};
		layerNames.push_back("VK_LAYER_KHRONOS_validation");

		// Application info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "MiniVk";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "MiniVk";
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
		DEV_ASSERT(vkCreateInstance(&instanceInfo, nullptr, &mInstance) == VK_SUCCESS, "CreateInstance", "Failed to create instance");

		// Handle debug messenger
#ifdef MINIVK_DEBUG

		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT"));

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = nullptr;

		DEV_ASSERT(debugMessengerCreateProc(mInstance, &debugMessengerInfo, nullptr, &mDebugMessenger) == VK_SUCCESS, "CreateInstance", "Failed to create debug messenger");
#endif
	}

	void Renderer::CreatePhysicalDevice()
	{
		// Get the physical devices count
		uint32 deviceCount = 0;
		DEV_ASSERT(vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr) == VK_SUCCESS, "CreatePhysicalDevice", "Failed to enumerate physical devices");
		DEV_ASSERT(deviceCount > 0, "CreatePhysicalDevice", "No physical devices found");

		// Get the physical devices
		Array<VkPhysicalDevice> devices(deviceCount);
		DEV_ASSERT(vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data()) == VK_SUCCESS, "CreatePhysicalDevice", "Failed to enumerate physical devices");

		for (auto& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties = {};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				mPhysicalDevice = device;
				break;
			}
		}

		if (mPhysicalDevice == VK_NULL_HANDLE)
		{
			mPhysicalDevice = devices[0];
		}
	}

	void Renderer::CreateLogicalDevice()
	{
		FindQueueFamilies();

		// Queue priority
		float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		Array<VkDeviceQueueCreateInfo> queueCreateInfos = {};

		// Graphics Queue Create Info
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
		graphicsQueueCreateInfo.queueCount = mGraphicsQueueFamily.QueueCapacity;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;

		queueCreateInfos.push_back(graphicsQueueCreateInfo);

		// Device Extensions
		Array<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// Device Features
		VkPhysicalDeviceFeatures deviceFeatures = {};

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

		DEV_ASSERT(vkCreateDevice(mPhysicalDevice, &deviceInfo, nullptr, &mDevice) == VK_SUCCESS, "CreateLogicalDevice", "Failed to create logical device");

		// Reserve the graphics queue
		mGraphicsQueueFamily.FreeQueues.reserve(mGraphicsQueueFamily.RequestedCount);

		for (uint32 i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mDevice, mGraphicsQueueFamily.FamilyIndex, i, &queue);
			mGraphicsQueueFamily.FreeQueues.push_back(queue);
		}

	}

	void Renderer::CreateSurfaceSwapchain()
	{
		// Create the surface
#ifdef MINIVK_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = GetModuleHandle(nullptr);
		surfaceInfo.hwnd = mWindowNativeHandle;

		DEV_ASSERT(vkCreateWin32SurfaceKHR(mInstance, &surfaceInfo, nullptr, &mSurface) == VK_SUCCESS, "CreateSurfaceSwapchain", "Failed to create surface");
#endif

		// Get the surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &surfaceCapabilities) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get surface capabilities");

#ifdef MINIVK_DEBUG
		CORE_LOG(HE_INFO, "Surface Capabilities", "Min Image Count: %d", surfaceCapabilities.minImageCount);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Max Image Count: %d", surfaceCapabilities.maxImageCount);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Current Extent: %d x %d", surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Min Image Extent: %d x %d", surfaceCapabilities.minImageExtent.width, surfaceCapabilities.minImageExtent.height);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Max Image Extent: %d x %d", surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Max Image Array Layers: %d", surfaceCapabilities.maxImageArrayLayers);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Supported Transforms: %d", surfaceCapabilities.supportedTransforms);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Current Transform: %d", surfaceCapabilities.currentTransform);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Supported Composite Alpha: %d", surfaceCapabilities.supportedCompositeAlpha);
		CORE_LOG(HE_INFO, "Surface Capabilities", "Supported Usage Flags: %d", surfaceCapabilities.supportedUsageFlags);
#endif

		// Get the surface formats
		uint32 formatCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get surface formats");
		DEV_ASSERT(formatCount > 0, "CreateSurfaceSwapchain", "No surface formats found");

		Array<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get surface formats");

#ifdef MINIVK_DEBUG
		for (auto& format : surfaceFormats)
		{
			CORE_LOG(HE_INFO, "Surface Formats", "Format: %d", format.format);
			CORE_LOG(HE_INFO, "Surface Formats", "Color Space: %d", format.colorSpace);
		}
#endif
		// Check if regular format is supported. If supports, use the regular format.
		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				mSwapchainImageFormat = format.format;
				colorSpace = format.colorSpace;
				break;
			}

			mSwapchainImageFormat = surfaceFormats[0].format;
			colorSpace = surfaceFormats[0].colorSpace;
		}

		// Get the present modes
		uint32 presentModeCount = 0;
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");
		DEV_ASSERT(presentModeCount > 0, "CreateSurfaceSwapchain", "No present modes found");

		Array<VkPresentModeKHR> presentModes(presentModeCount);
		DEV_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, mSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");

#ifdef MINIVK_DEBUG
		for (auto& mode : presentModes)
		{
			CORE_LOG(HE_INFO, "Present Modes", "Mode: %d", mode);
		}
#endif

		// Check if the surface is supported by the physical device
		uint32 presentFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
		QueueDesc queueDesc = {};
		queueDesc.QueueFamilyIndex = presentFamilyIndex;
		queueDesc.Type = QueueType::Present;
		mGraphicsQueue = CreateQueue(queueDesc);
		VkBool32 presentSupport = false;

		DEV_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, presentFamilyIndex, mSurface, &presentSupport) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present support");
		DEV_ASSERT(presentSupport, "CreateSurfaceSwapchain", "Present support not found");

		// Create the swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mSurface;
		swapchainInfo.minImageCount = surfaceCapabilities.minImageCount;
		swapchainInfo.imageFormat = mSwapchainImageFormat;
		swapchainInfo.imageColorSpace = colorSpace;
		swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = &presentFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentModes[0];
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateSwapchainKHR(mDevice, &swapchainInfo, nullptr, &mSwapchain) == VK_SUCCESS, "CreateSurfaceSwapchain", "Failed to create swapchain");
	}

	void Renderer::CreateSwapchainImages()
	{
		// Get the swapchain images
		uint32 imageCount = 0;
		DEV_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr) == VK_SUCCESS, "CreateSwapchainImages", "Failed to get swapchain images");
		DEV_ASSERT(imageCount > 0, "CreateSwapchainImages", "No swapchain images found");

		mSwapchainImages.resize(imageCount);
		mSwapchainImageViews.resize(imageCount);
		DEV_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, mSwapchainImages.data()) == VK_SUCCESS, "CreateSwapchainImages", "Failed to get swapchain images");

		// Create image view for each swapchain image
		for (int i = 0; i < imageCount; i++)
		{
			VkImageViewCreateInfo imageViewInfo = {};
			imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewInfo.image = mSwapchainImages[i];
			imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			imageViewInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
			imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewInfo.subresourceRange.baseMipLevel = 0;
			imageViewInfo.subresourceRange.levelCount = 1;
			imageViewInfo.subresourceRange.baseArrayLayer = 0;
			imageViewInfo.subresourceRange.layerCount = 1;

			DEV_ASSERT(vkCreateImageView(mDevice, &imageViewInfo, nullptr, &mSwapchainImageViews[i]) == VK_SUCCESS, "Create Swapchain views", "Failed to create image view for swapchain");
		}
	}

	void Renderer::CreateFence()
	{
		mFences.resize(mSwapchainImages.size());

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for(int i = 0; i < mSwapchainImages.size(); i++)
		{
			DEV_ASSERT(vkCreateFence(mDevice, &fenceInfo, nullptr, &mFences[i]) == VK_SUCCESS, "CreateFence", "Failed to create fence");
		}
	}

	void Renderer::CreateSemaphores()
	{
		mImageSemaphores.resize(mSwapchainImages.size());
		mRenderSemaphores.resize(mSwapchainImages.size());
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = VkSemaphoreCreateFlags();

		for (int i = 0; i < mSwapchainImages.size(); i++)
		{
			DEV_ASSERT(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mImageSemaphores[i]) == VK_SUCCESS, "CreateSemaphores", "Failed to create image available semaphore");
			DEV_ASSERT(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mRenderSemaphores[i]) == VK_SUCCESS, "CreateSemaphores", "Failed to create render finished semaphore");
		}
	}

	void Renderer::CreateCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;

		DEV_ASSERT(vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool) == VK_SUCCESS, "CreateCommandBuffer", "Failed to create command pool");
	}

	void Renderer::CreateCommandBuffer()
	{
		mCommandBuffers.resize(mSwapchainImages.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = mSwapchainImages.size();

		DEV_ASSERT(vkAllocateCommandBuffers(mDevice, &allocInfo, mCommandBuffers.data()) == VK_SUCCESS, "CreateCommandBuffer", "Failed to allocate command buffer");
	}

	Shader* Renderer::CreateShader(const ShaderDesc& desc)
	{
		return new Shader(desc, this);
	}

	Pipeline* Renderer::CreatePipeline(const GraphicsPipelineDesc& desc)
	{
		return new Pipeline(desc, this);
	}

	RenderPass* Renderer::CreateRenderPass(const RenderPassDesc& desc)
	{
		return new RenderPass(desc, this);
	}

	Buffer* Renderer::CreateBuffer(const BufferDesc& desc)
	{
		return new Buffer(desc, this);
	}

	Queue* Renderer::CreateQueue(const QueueDesc& desc)
	{
		return new Queue(desc, this);
	}

	void Renderer::FindQueueFamilies()
	{
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);
		DEV_ASSERT(queueFamilyCount > 0, "FindQueueFamilies", "No queue families found");

		Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int index = 0;
		for (auto& prop : queueFamilies)
		{
			DEV_LOG(HE_WARNING, "Queue Family Index: %d", index);

			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsQueueFamily.FamilyIndex == 255)
			{
				DEV_LOG(HE_INFO, "Queue Graphics Found. Count: %d", prop.queueCount);
				mGraphicsQueueFamily.FamilyIndex = index;
				mGraphicsQueueFamily.QueueCapacity = prop.queueCount;
				mGraphicsQueueFamily.RequestedCount = 1;
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
			{
				DEV_LOG(HE_INFO, "Queue Compute Found. Count: %d", prop.queueCount);
				mComputeQueueFamily.FamilyIndex = index;
				mComputeQueueFamily.QueueCapacity = prop.queueCount;
				mComputeQueueFamily.RequestedCount = 1;
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
			{
				DEV_LOG(HE_INFO, "Queue Transfer Found. Count: %d", prop.queueCount);
				mTransferQueueFamily.FamilyIndex = index;
				mTransferQueueFamily.QueueCapacity = prop.queueCount;
				mTransferQueueFamily.RequestedCount = 1;
			}
			index++;
		}
	}
}