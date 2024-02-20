#include <Runtime/Core/Core.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>


namespace Hollow
{
	void RenderPassTest();
	void TriangleTest();
	void PostProcessTest();

	struct TestVertex
	{
		Vector3f Position;
		Vector2f TexCoord;
	};

	struct TestConstantBuffer
	{
		Matrix4f Transform;
		Matrix4f View;
		Matrix4f Projection;
	};

	Array<TestVertex> TestTriangleVtx;
	Array<uint16> TestTriangleIdx;
	Array<TestConstantBuffer> TestTriangleCB;
}

int main()
{
	Hollow::RenderPassTest();
	return 0;
}

namespace Hollow
{
	void RenderPassTest()
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

		// Create Queue
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

		// Test Texture
		TextureDesc textureDesc;
		textureDesc.ArraySize = 1;
		textureDesc.Format = TextureFormat::RGBA8_UNorm;
		textureDesc.ImageSize = { 800, 600, 1 };
		textureDesc.MipLevels = 1;
		textureDesc.SampleCount = TextureSample::Sample1;
		textureDesc.Usages = { TextureUsage::ColorAttachment };
		textureDesc.CPUAccess = BufferCPUAccess::Write;
		textureDesc.Type = TextureType::Texture2D;

		auto mTexture = mDevice->CreateTexture(textureDesc);


		while (!mGameWindow->ShouldClose())
		{
			mGameWindow->PollMessages();
		}
	}

	void TriangleTest()
	{
	}

	void PostProcessTest()
	{
	}
}