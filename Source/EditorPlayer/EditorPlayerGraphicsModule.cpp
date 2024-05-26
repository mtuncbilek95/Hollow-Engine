#include "EditorPlayerGraphicsModule.h"

#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Window/WindowManager.h>

namespace Hollow
{
	EditorPlayerGraphicsModule::EditorPlayerGraphicsModule()
	{
	}

	void EditorPlayerGraphicsModule::Start() noexcept
	{
	}

	void EditorPlayerGraphicsModule::Update() noexcept
	{
	}

	void EditorPlayerGraphicsModule::Stop() noexcept
	{
	}

	void EditorPlayerGraphicsModule::OnShutdown() noexcept
	{
	}

	bool EditorPlayerGraphicsModule::OnInitialize() noexcept
	{
		// Create a graphics instance
		GraphicsInstanceDesc instanceDesc = {};
		instanceDesc.API = GraphicsAPI::Vulkan;
		instanceDesc.ApplicationName = "Hollow Engine";
		instanceDesc.InstanceName = "Hollow Engine";
		mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);

		if (!mGraphicsInstance)
			return false;

		// Create a graphics device
		GraphicsDeviceDesc deviceDesc = {};
		deviceDesc.Instance = mGraphicsInstance;
		deviceDesc.GraphicsQueueCount = 1;
		deviceDesc.ComputeQueueCount = 1;
		deviceDesc.TransferQueueCount = 1;
		mGraphicsDevice = mGraphicsInstance->CreateGraphicsDevice(deviceDesc);

		if (!mGraphicsDevice)
			return false;

		// Create a swapchain
		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 2;
		swapchainDesc.ImageSize = WindowManager::GetInstanceAPI().GetDefaultWindow()->GetWindowResolution();
		swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
		swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
		swapchainDesc.VSync = PresentMode::FullVSync;
		swapchainDesc.SwapchainMode = ShareMode::Exclusive;
		swapchainDesc.pQueue = GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue();

		mSwapchain = mGraphicsDevice->CreateSwapchain(swapchainDesc);

		if (!mSwapchain)
			return false;
		
		return true;
	}

	void EditorPlayerGraphicsModule::OnPreUpdate() noexcept
	{
	}

	void EditorPlayerGraphicsModule::OnPostUpdate() noexcept
	{
	}
}
