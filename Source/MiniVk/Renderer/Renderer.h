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
	class Buffer;
	class Queue;
	class DescriptorSet;
	class DescriptorPool;
	class DescriptorLayout;

	struct ShaderDesc;
	struct GraphicsPipelineDesc;
	struct RenderPassDesc;
	struct BufferDesc;
	struct QueueDesc;
	struct DescriptorSetDesc;
	struct DescriptorPoolDesc;
	struct DescriptorLayoutDesc;

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
		VkQueue GetFreeComputeQueue() { return mComputeQueueFamily.GetFreeQueue(); }
		VkQueue GetFreeTransferQueue() { return mTransferQueueFamily.GetFreeQueue(); }

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
		Buffer* CreateBuffer(const BufferDesc& desc);
		Queue* CreateQueue(const QueueDesc& desc);
		DescriptorSet* CreateDescriptorSet(const DescriptorSetDesc& desc);
		DescriptorPool* CreateDescriptorPool(const DescriptorPoolDesc& desc);
		DescriptorLayout* CreateDescriptorLayout(const DescriptorLayoutDesc& desc);

		Vector2u GetWindowSize() const { return mWindowSize; }

		void BindBuffer(Buffer* buffer, void* inData);
		void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer, uint32 sizeInBytes);
		void BindDescriptorSet(Pipeline* pipeline, DescriptorSet** pDescriptorSet, uint32 amount, uint32 imageIndex);
		void UpdateDescriptorSet(Buffer** pBuffers, DescriptorSet** pDescriptorSet, uint32 setCount, uint32 imageIndex, uint32 range);

		void BeginRecording(uint32* imageIndex);
		void BindRenderPass(Pipeline* pipeline, uint32 imageIndex);
		void Draw(Buffer* pBuffer, uint32 vertexSize, uint32 imageIndex);
		void DrawIndexed(Buffer* pVertex, Buffer* pIndex, uint32 vertexSize, uint32 indexSize, uint32 imageIndex);
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
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;

		Queue* mGraphicsQueue;
		Queue* mComputeQueue;
		Queue* mTransferQueue;

	private:
		Array<VkImage> mSwapchainImages;
		Array<VkImageView> mSwapchainImageViews;

		VkFormat mSwapchainImageFormat;

		Array<VkCommandBuffer> mCommandBuffers;
		VkCommandPool mCommandPool;

		VkCommandBuffer mCommandDrawBuffer;

		Array<VkFence> mFences;
		Array<VkSemaphore> mImageSemaphores;
		Array<VkSemaphore> mRenderSemaphores;

	};
}