#include <Runtime/Core/Core.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>


namespace Hollow
{
	void ClearColorTest();
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

	ArrayList<TestVertex> TestTriangleVtx;
	ArrayList<uint16> TestTriangleIdx;
	ArrayList<TestConstantBuffer> TestTriangleCB;
}

int main()
{
	DEV_LOG(HE_INFO, "Hello, World!");
	return 0;
}

namespace Hollow
{
	void ClearColorTest()
	{
		WindowDesc windowDesc;
		windowDesc.WindowPosition = { 1000, 100 };
		windowDesc.WindowSize = { 800, 600 };
		windowDesc.WindowTitle = "Hollow Engine";
		windowDesc.Windowed = true;

		auto mGameWindow = WindowManager::GetInstance().CreateAppWindow(windowDesc);

		GraphicsInstanceDesc instanceDesc;
		instanceDesc.API = GraphicsAPI::D3D11;
		auto mInstance = GraphicsInstance::Create(instanceDesc);

		auto mAdapter = mInstance->GetAdapter(0);

		GraphicsDeviceDesc deviceDesc;
		deviceDesc.Adapter = mAdapter;
		deviceDesc.API = GraphicsAPI::D3D11;

		mAdapter->CreateDevice(deviceDesc);

		auto mGraphicsDevice = mAdapter->GetDevice();

		SwapchainDesc swapchainDesc;
		swapchainDesc.BufferCount = 3;
		swapchainDesc.pInstance = mInstance;
		swapchainDesc.SwapchainFormat = TextureFormat::RGBA8_UNorm;
		swapchainDesc.VSync = true;
		swapchainDesc.Windowed = true;
		swapchainDesc.SampleCount = 1;

		auto mSwapchain = mGraphicsDevice->CreateSwapchain(swapchainDesc);
	}
}