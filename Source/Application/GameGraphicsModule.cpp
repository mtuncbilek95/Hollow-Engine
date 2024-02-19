#include "GameGraphicsModule.h"

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Window/WindowManager.h>

namespace Hollow
{
	GameGraphicsModule::GameGraphicsModule()
	{
		SetModuleName("GameGraphicsModule");
	}

	void GameGraphicsModule::Start()
	{
	}

	void GameGraphicsModule::Update()
	{
		mSwapchain->Present();
	}

	void GameGraphicsModule::Stop()
	{
	}

	bool GameGraphicsModule::OnInitialize()
	{
		GraphicsInstanceDesc instanceDesc;
		instanceDesc.API = GraphicsAPI::D3D11;
		mInstance = GraphicsInstance::Create(instanceDesc);

		mAdapter = mInstance->GetAdapter(0);

		GraphicsDeviceDesc deviceDesc;
		deviceDesc.Adapter = mAdapter;
		deviceDesc.API = GraphicsAPI::D3D11;

		mAdapter->CreateDevice(deviceDesc);

		mGraphicsDevice = mAdapter->GetDevice();

		SwapchainDesc swapchainDesc;
		swapchainDesc.BufferCount = 3;
		swapchainDesc.SwapchainFormat = TextureFormat::RGBA8_UNorm;
		swapchainDesc.VSync = PresentMode::VSyncImmediate;
		swapchainDesc.SampleCount = 1;
		swapchainDesc.pQueue = nullptr;

		mSwapchain = mGraphicsDevice->CreateSwapchain(swapchainDesc);
		return true;
	}

	void GameGraphicsModule::OnPreUpdate()
	{
	}

	void GameGraphicsModule::OnPostUpdate()
	{
	}

	void GameGraphicsModule::OnShutdown()
	{
		CORE_LOG(HE_INFO, "GameGraphicsModule", "Shutting down.");
	}
}
