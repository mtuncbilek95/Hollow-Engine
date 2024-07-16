#include <Engine/Platform/PlatformAPI.h>
#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/Graphics/Queue/GraphicsQueue.h>
#include <Engine/Vulkan/Queue/VQueue.h>

#include <Engine/Graphics/Memory/GraphicsMemory.h>

using namespace Hollow;

#include <iostream>

int main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI()->InitializeArguments(argC, argV);
	GraphicsAPI* GraphicsManager = GraphicsAPI::GetAPI();

	WindowDesc windowDesc = {};
	windowDesc.WindowMode = WindowMode::Windowed;
	windowDesc.WindowPosition = { 100, 100 };
	windowDesc.WindowSize = { 800, 600 };
	windowDesc.WindowTitle = "Hollow Editor";
	SharedPtr<PlatformWindow> mWindow = WindowManager::GetAPI()->InitializeWindow(windowDesc);

	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.AppName = "Editor";
	instanceDesc.AppVersion = { 1, 0, 0 };
	instanceDesc.EngineName = "Hollow";
	instanceDesc.EngineVersion = { 1, 0, 0 };

	SharedPtr<GraphicsInstance> mInstance = GraphicsManager->CreateInstance(instanceDesc);
	SharedPtr<GraphicsDevice> mDevice = GraphicsManager->CreateDevice();

	SwapchainDesc swapchainDesc = {};
	swapchainDesc.ImageSize = { 800, 600 };
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.TextureUsage = TextureUsageFlags::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.pRequestQueue = GraphicsManager->GetGraphicsQueue();
	SharedPtr<Swapchain> mSwapchain = GraphicsManager->CreateSwapchain(swapchainDesc);



	return 0;
}
