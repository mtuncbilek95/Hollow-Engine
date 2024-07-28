#include "GraphicsModule.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Window/WindowAPI.h>

#include <Engine/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	GraphicsModule::GraphicsModule()
	{
		SetModuleName("GraphicsModule");
		mGraphicsAPI = GraphicsAPI::GetAPI();
	}

	GraphicsModule::~GraphicsModule()
	{
	}

	void GraphicsModule::Start() noexcept
	{
	}

	void GraphicsModule::Update() noexcept
	{
	}

	void GraphicsModule::Stop() noexcept
	{
		mGraphicsAPI->GetDevice().lock()->WaitIdle();
	}

	bool GraphicsModule::OnInitialize() noexcept
	{

		GraphicsInstanceDesc instanceDesc = {};
		instanceDesc.AppName = "Editor"; // TODO: Get from config
		instanceDesc.EngineName = "Hollow"; // TODO: Get from config
		instanceDesc.AppVersion = Vec3b(1, 0, 0); // TODO: Get from config
		instanceDesc.EngineVersion = Vec3b(1, 0, 0); // TODO: Get from config

		auto tempRefInstance = mGraphicsAPI->CreateInstance(instanceDesc);
		if (tempRefInstance.expired())
			return false;

		auto tempRefDevice = mGraphicsAPI->CreateDevice();
		if (tempRefDevice.expired())
			return false;

		SwapchainDesc swapchainDesc = {};
		swapchainDesc.BufferCount = 2; // TODO: Get from config
		swapchainDesc.ImageFormat = TextureFormat::RGBA8_UNorm; // TODO: Get from config
		swapchainDesc.TextureUsage = TextureUsageFlags::ColorAttachment; // TODO: Get from config
		swapchainDesc.VSync = PresentMode::FullVSync; // TODO: Get from config
		swapchainDesc.ImageSize = WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowResolution();
		swapchainDesc.pRequestQueue = mGraphicsAPI->GetGraphicsQueue();

		auto tempRefSwapchain = mGraphicsAPI->CreateSwapchain(swapchainDesc);
		if (tempRefSwapchain.expired())
			return false;

		return true;
	}

	void GraphicsModule::OnPreUpdate() noexcept
	{
	}

	void GraphicsModule::OnPostUpdate() noexcept
	{
	}
}
