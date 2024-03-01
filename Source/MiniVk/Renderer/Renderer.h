#pragma once

#include <Core/Core.h>

#include <vulkan.h>

#ifdef MINIVK_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#include <Windows.h>
#include <Vulkan/Include/vulkan_win32.h>
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <GLFW/glfw3.h>

namespace MiniVk
{
	class Shader;
	class Pipeline;
	class RenderPass;

	struct ShaderDesc;
	struct GraphicsPipelineDesc;
	struct RenderPassDesc;

	struct RendererDesc
	{
		HWND WindowNativeHandle;
		GLFWwindow* WindowHandle;
		Vector2u WindowSize;
	};

	struct QueueFamily
	{
		QueueFamily() : QueueCapacity(0), FamilyIndex(255), RequestedCount(0) {}

		bool HasFreeQueue() const { return FreeQueues.size() > 0; }

		VkQueue GetFreeQueue()
		{
			if (FreeQueues.size() > 0)
			{
				VkQueue pQ = FreeQueues[0];
				FreeQueues.erase(FreeQueues.begin());
				return pQ;
			}
			return VK_NULL_HANDLE;
		}

		byte QueueCapacity;
		byte FamilyIndex;
		byte RequestedCount;
		Array<VkQueue> FreeQueues;
	};

	class Renderer
	{
	public:
		Renderer(const RendererDesc& desc);
		~Renderer() = default;

		void Initialize();
		void Shutdown();

		VkQueue GetFreeGraphicsQueue() { return mGraphicsQueueFamily.GetFreeQueue(); }

		VkInstance GetVkInstance() const { return mInstance; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mPhysicalDevice; }
		VkDevice GetVkLogicalDevice() const { return mDevice; }
		VkSurfaceKHR GetVkSurface() const { return mSurface; }
		VkSwapchainKHR GetVkSwapchain() const { return mSwapchain; }
		VkFormat GetVkSwapchainImageFormat() const { return mSwapchainImageFormat; }
		Array<VkImage> GetVkSwapchainImages() const { return mSwapchainImages; }
		Array<VkImageView> GetVkSwapchainImageViews() const { return mSwapchainImageViews; }

		Shader* CreateShader(const ShaderDesc& desc);
		Pipeline* CreatePipeline(const GraphicsPipelineDesc& desc);
		RenderPass* CreateRenderPass(const RenderPassDesc& desc);

		Vector2u GetWindowSize() const { return mWindowSize; }

		void BeginRecording(uint32* imageIndex);
		void BindRenderPass(Pipeline* pipeline, uint32 imageIndex);
		void Draw(uint32 imageIndex);
		void EndRecording(uint32 imageIndex);
		void Present(uint32 imageIndex);
	protected:
		void CreateInstance();
		void CreatePhysicalDevice();
		void CreateLogicalDevice();
		void CreateSurfaceSwapchain();
		void CreateSwapchainImages();
		void CreateFence();
		void CreateSemaphores();

		void CreateCommandPool();
		void CreateCommandBuffer();

	private:
		static PFN_vkCmdBeginRendering vkCmdBeginRendering;
		static PFN_vkCmdEndRendering vkCmdEndRendering;

		void FindQueueFamilies();

	private:
		VkInstance mInstance;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;
		VkSurfaceKHR mSurface;
		VkSwapchainKHR mSwapchain;

		HWND mWindowNativeHandle;
		GLFWwindow* mWindowHandle;
		Vector2u mWindowSize;

#ifdef MINIVK_DEBUG
		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
		QueueFamily mGraphicsQueueFamily;
		VkQueue mGraphicsQueue;

	private:
		Array<VkImage> mSwapchainImages;
		Array<VkImageView> mSwapchainImageViews;

		VkFormat mSwapchainImageFormat;

		Array<VkCommandBuffer> mCommandBuffers;
		VkCommandPool mCommandPool;

		Array<VkFence> mFences;
		Array<VkSemaphore> mImageSemaphores;
		Array<VkSemaphore> mRenderSemaphores;

	};
}