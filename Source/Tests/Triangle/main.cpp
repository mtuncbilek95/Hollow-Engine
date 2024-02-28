#include <Runtime/Core/Core.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Texture/Texture.h>

namespace Hollow
{
	void VertexTriangleTest();

	struct TestVertex
	{
		Vector3f Position;
		Vector4f Color;
	};

	struct TestConstantBuffer
	{
		Matrix4f Transform;
		Matrix4f View;
		Matrix4f Projection;
	};

	Array<TestVertex> TestTriangleVtx;
	Array<uint16> TestTriangleIdx;
	Array<TestConstantBuffer> TestTriangleMVP;
}

int main()
{
	Hollow::VertexTriangleTest();
	return 0;
}

namespace Hollow
{
	void VertexTriangleTest()
	{
		// Create Window
		WindowDesc windowDesc;
		windowDesc.WindowPosition = { 1000, 100 };
		windowDesc.WindowSize = { 800, 600 };
		windowDesc.WindowTitle = "Hollow Engine";
		windowDesc.Windowed = true;

		auto mGameWindow = WindowManager::GetInstance().CreateAppWindow(windowDesc);
		
		// Create Instance
		GraphicsInstanceDesc instanceDesc;
		instanceDesc.API = GraphicsAPI::Vulkan;
		auto mInstance = GraphicsInstance::Create(instanceDesc);

		// Create Adapter
		auto mAdapter = mInstance->GetAdapter(0);

		// Create Device
		VulkanDeviceDesc deviceDesc;
		deviceDesc.Adapter = mAdapter;
		deviceDesc.API = GraphicsAPI::Vulkan;
		deviceDesc.deviceExtensions = {};
		deviceDesc.deviceFeatures = {};
		
		mAdapter->CreateDevice(deviceDesc);

		auto mDevice = mAdapter->GetDevice();

		// Create Swapchain Queue
		GraphicsQueueDesc queueDesc;
		queueDesc.Type = GraphicsQueueType::Graphics;
		auto mQueue = mDevice->BorrowGraphicsQueue(queueDesc);

		// Create Swapchain
		SwapchainDesc swapchainDesc;
		swapchainDesc.BufferCount = 2;
		swapchainDesc.SwapchainFormat = TextureFormat::RGBA8_UNorm;
		swapchainDesc.VSync = PresentMode::VSyncQueued;
		swapchainDesc.pQueue = mQueue.get();
		auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

		// Create CommandPool
		CommandPoolDesc poolDesc;
		poolDesc.Type = CommandPoolType::Graphics;
		auto mCommandPool = mDevice->CreateCommandPool(poolDesc);

		// Create CommandBuffer
		CommandBufferDesc cmdDesc = {};
		cmdDesc.pCommandPool = mCommandPool.get();
		auto mCommandBuffer = mDevice->CreateCommandBuffer(cmdDesc);

		// Create Fence
		FenceDesc fenceDesc;
		fenceDesc.bSignalled = false;
		auto mFence = mDevice->CreateFence(fenceDesc);

		mSwapchain->GetColorImages();

		Array<SharedPtr<Texture>> mSwapchainTextures = mSwapchain->GetColorImages();
		mCommandBuffer->

		byte presentIndex = 0;
		while (!mGameWindow->ShouldClose())
		{
			mGameWindow->PollMessages();

			mSwapchain->Present(nullptr, 0);
			mSwapchain->WaitForPresent(presentIndex);
			presentIndex = (presentIndex + 1) % swapchainDesc.BufferCount;
		}
	}
}