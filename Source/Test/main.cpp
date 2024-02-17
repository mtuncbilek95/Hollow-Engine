#include <Runtime/Core/Core.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>

#include <Runtime/D3D11/Device/D3D11Device.h>
#include <Runtime/D3D11/RenderPass/D3D11RenderPass.h>
#include <Runtime/D3D11/Swapchain/D3D11Swapchain.h>


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
		instanceDesc.API = GraphicsAPI::D3D11;
		auto mInstance = GraphicsInstance::Create(instanceDesc);

		// Get Adapter
		auto mAdapter = mInstance->GetAdapter(0);

		// Create Device
		GraphicsDeviceDesc deviceDesc;
		deviceDesc.Adapter = mAdapter;
		deviceDesc.API = GraphicsAPI::D3D11;

		mAdapter->CreateDevice(deviceDesc);

		auto mGraphicsDevice = mAdapter->GetDevice();

		// Create Swapchain
		SwapchainDesc swapchainDesc;
		swapchainDesc.BufferCount = 2;
		swapchainDesc.pInstance = mInstance;
		swapchainDesc.SwapchainFormat = TextureFormat::RGBA8_UNorm;
		swapchainDesc.VSync = true;
		swapchainDesc.Windowed = true;
		swapchainDesc.SampleCount = 1;

		auto mSwapchain = mGraphicsDevice->CreateSwapchain(swapchainDesc);

		// Create Texture For Color Attachment
		TextureDesc colorAttachmentDesc;
		colorAttachmentDesc.ImageSize = { 800, 600 };
		colorAttachmentDesc.Format = TextureFormat::RGBA8_UNorm;
		colorAttachmentDesc.ArraySize = 1;
		colorAttachmentDesc.MipLevels = 0;
		colorAttachmentDesc.Type = TextureType::Texture2D;
		colorAttachmentDesc.Usage = TextureUsage::RenderTarget;
		colorAttachmentDesc.CPUAccess = BufferCPUAccess::Write;

		auto mRenderTarget = mGraphicsDevice->CreateTexture(colorAttachmentDesc);

		// Create Texture For Depth Attachment
		TextureDesc depthAttachmentDesc;
		depthAttachmentDesc.ImageSize = { 800, 600 };
		depthAttachmentDesc.Format = TextureFormat::D24_UNorm_S8_UInt;
		depthAttachmentDesc.ArraySize = 1;
		depthAttachmentDesc.MipLevels = 0;
		depthAttachmentDesc.Type = TextureType::Texture2D;
		depthAttachmentDesc.Usage = TextureUsage::DepthStencil;
		depthAttachmentDesc.CPUAccess = BufferCPUAccess::None;

		auto mDepthStencil = mGraphicsDevice->CreateTexture(depthAttachmentDesc);

		// Create ColorAttachment
		RenderPassAttachmentDesc colorAttachment;
		colorAttachment.pTexture = mRenderTarget;
		colorAttachment.ArrayLevel = 1;
		colorAttachment.MipLevel = 0;

		// Create DepthAttachment
		RenderPassAttachmentDesc depthAttachment;
		depthAttachment.pTexture = mDepthStencil;
		depthAttachment.ArrayLevel = 1;
		depthAttachment.MipLevel = 0;

		// Create RenderPass
		RenderPassDesc renderPassDesc;
		renderPassDesc.ColorAttachments.push_back(colorAttachment);
		renderPassDesc.DepthStencilAttachment = depthAttachment;
		renderPassDesc.FramebufferSize = { 800, 600 };
		renderPassDesc.IsSwapchainTarget = true;

		auto mRenderPass = mGraphicsDevice->CreateRenderPass(renderPassDesc);

		while (!mGameWindow->ShouldClose())
		{
			mGameWindow->PollMessages();
			mSwapchain->Present();
		}
	}

	void TriangleTest()
	{
	}

	void PostProcessTest()
	{
	}
}