#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>

using namespace Hollow;

int main()
{
	// Create a window
	WindowDesc winDesc = {};
	winDesc.Windowed = true;
	winDesc.WindowSize = { 1280, 720 };
	winDesc.WindowTitle = "Test Application";
	winDesc.WindowPosition = { 100, 100 };

	auto mWindow = WindowManager::GetInstance().CreateAppWindow(winDesc);

	// Create a graphics instance
	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.InstanceName = "Hollow Engine";
	instanceDesc.ApplicationName = "Test Application";
	instanceDesc.EnabledExtensions = {};

	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);
	mGraphicsInstance->EnumerateAdapters();

	// Create a graphics device
	auto mDevice = mGraphicsInstance->CreateDevice();

	// Create a present queue for swapchain
	GraphicsQueueDesc presentQueueDesc = {};
	presentQueueDesc.QueueType = GraphicsQueueType::Graphics;

	auto mPresentQueue = mDevice->CreateGraphicsQueue(presentQueueDesc);

	// Create swapchain for the window
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = winDesc.WindowSize;
	swapchainDesc.pQueue = mPresentQueue.get();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	// Create present semaphore for each frame
	Array<SharedPtr<Semaphore>> presentSemaphores(swapchainDesc.BufferCount);
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		presentSemaphores[i] = mDevice->CreateSyncSemaphore();
	}

	// Create image semaphore for each frame
	Array<SharedPtr<Semaphore>> imageSemaphores(swapchainDesc.BufferCount);
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		imageSemaphores[i] = mDevice->CreateSyncSemaphore();
	}

	// ----------------- CREATE SHADERS -----------------

	// Create vertex shader
	ShaderDesc vsDesc = {};
	vsDesc.EntryPoint = "main";
	vsDesc.Language = ShaderLanguage::GLSL;
	vsDesc.ShaderName = "Test Vertex Shader";
	vsDesc.Source = ShaderCompiler::GetShaderCodeAsString(R"(D:\Projects\Another\Hollow-Engine\Shaders\testVertex.vert)");
	vsDesc.Stage = ShaderStage::Vertex;
	
	auto mVertexShader = mDevice->CreateShader(vsDesc);

	// Create fragment shader
	ShaderDesc fsDesc = {};
	fsDesc.EntryPoint = "main";
	fsDesc.Language = ShaderLanguage::GLSL;
	fsDesc.ShaderName = "Test Fragment Shader";
	fsDesc.Source = ShaderCompiler::GetShaderCodeAsString(R"(D:\Projects\Another\Hollow-Engine\Shaders\testFrag.frag)");
	fsDesc.Stage = ShaderStage::Fragment;

	auto mFragmentShader = mDevice->CreateShader(fsDesc);

	// ----------------- CREATE RENDERPASS -----------------

	// Get Attachment Descriptions
	Array<RenderPassAttachmentDesc> attachments;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		RenderPassAttachmentDesc colorAttachment = {};
		colorAttachment.Format = TextureFormat::RGBA8_UNorm;
		colorAttachment.SampleCount = TextureSampleCount::Sample1;
		colorAttachment.ColorLoadOperation = RenderPassLoadOperation::Clear;
		colorAttachment.ColorStoreOperation = RenderPassStoreOperation::Store;
		colorAttachment.StencilLoadOperation = RenderPassLoadOperation::Ignore;
		colorAttachment.StencilStoreOperation = RenderPassStoreOperation::Ignore;
		colorAttachment.InputLayout = TextureMemoryLayout::ColorAttachment;
		colorAttachment.OutputLayout = TextureMemoryLayout::Present;
		colorAttachment.ArrayLevel = 1;
		colorAttachment.MipLevel = 1;
		colorAttachment.pView = mSwapchain->GetImageViews()[i].get();

		attachments.push_back(colorAttachment);
	}

	// Create RenderPass
	RenderPassDesc renderPassDesc = {};
	renderPassDesc.TargetRenderSize = winDesc.WindowSize;
	renderPassDesc.ColorAttachments = attachments;
	renderPassDesc.pDepthStencilAttachment = nullptr;

	auto mRenderPass = mDevice->CreateRenderPass(renderPassDesc);

	// ----------------- CREATE PIPELINE -----------------
	
	// BlendStateAttachment	
	BlendStateAttachment blendAttachment = {};
	blendAttachment.bEnabled = false;
	blendAttachment.ColorOperation = BlendOperation::Add;
	blendAttachment.AlphaOperation = BlendOperation::Add;
	blendAttachment.SourceColorFactor = BlendFactor::One;
	blendAttachment.DestinationColorFactor = BlendFactor::Zero;
	blendAttachment.SourceAlphaFactor = BlendFactor::One;
	blendAttachment.DestinationAlphaFactor = BlendFactor::Zero;
	blendAttachment.WriteMask = BlendColorWriteMask::All;

	// Create BlendState
	BlendStateDesc blendState = {};
	blendState.bLogicOperationEnabled = false;
	blendState.LogicOperation = LogicOperation::Copy;
	blendState.Attachments = { blendAttachment };

	// Create DepthStencilState
	DepthStencilStateDesc depthStencilState = {};
	depthStencilState.bDepthTestEnabled = false;
	depthStencilState.bDepthWriteEnabled = false;
	depthStencilState.bStencilTestEnabled = false;
	depthStencilState.DepthTestOperation = CompareOperation::Always;
	depthStencilState.StencilBackFace = {};
	depthStencilState.StencilFrontFace = {};
	
	// Position InputElement
	InputElement inputElement1 = {};
	inputElement1.Format = TextureFormat::RG32_Float;
	inputElement1.Semantic = InputElementSemantic::Position;

	// Color InputElement
	InputElement inputElement2 = {};
	inputElement1.Format = TextureFormat::RGBA32_Float;
	inputElement1.Semantic = InputElementSemantic::Color;
	
	// InputBinding
	InputBinding inputBinding = {};
	inputBinding.StepRate = InputBindingStepRate::Vertex;
	inputBinding.Elements = { inputElement1, inputElement2 };

	// Create InputLayout
	InputLayoutDesc inputLayout = {};
	inputLayout.Bindings = { inputBinding };
	inputLayout.Topology = MeshTopology::TriangleList;

	// Create MultisampleDesc
	MultisampleDesc multisample = {};
	multisample.bSampleShadingEnabled = false;
	multisample.Samples = TextureSampleCount::Sample1;

	// Create RasterizerState
	RasterizerStateDesc rasterizerState = {};
	rasterizerState.bDepthBiasEnabled = false;
	rasterizerState.bFrontCounterClockwise = false;
	rasterizerState.CullFlags = FaceCullMode::Back;
	rasterizerState.FillMode = PolygonMode::Fill;
	rasterizerState.DepthBiasClamp = 0.0f;
	rasterizerState.DepthBiasFactor = 0.0f;
	rasterizerState.DepthBiasSlope = 0.0f;

	// Create Viewport
	ViewportDesc viewport = {};
	viewport.DepthRange = { 0.0f, 1.0f };
	viewport.OffsetSize = { 0, 0 };
	viewport.ViewportSize = winDesc.WindowSize;

	// Create Scissor
	ScissorDesc scissor = {};
	scissor.OffsetSize = { 0, 0 };
	scissor.ScissorSize = winDesc.WindowSize;

	// Create Pipeline
	GraphicsPipelineDesc pipelineDesc = {};
	pipelineDesc.BlendState = blendState;
	pipelineDesc.DepthStencilState = depthStencilState;
	pipelineDesc.InputLayout = inputLayout;
	pipelineDesc.Multisample = multisample;
	pipelineDesc.RasterizerState = rasterizerState;
	pipelineDesc.ResourceLayout = {};
	pipelineDesc.GraphicsShaders = { mVertexShader.get(), mFragmentShader.get() };
	pipelineDesc.pRenderPass = mRenderPass.get();
	pipelineDesc.Viewport = viewport;
	pipelineDesc.Scissor = scissor;
	pipelineDesc.SubpassIndex = 0;

	auto mPipeline = mDevice->CreateGraphicsPipeline(pipelineDesc);

	// ----------------- RENDER LOOP -----------------

	while (!mWindow->ShouldClose())
	{
		Semaphore* presentSemaphore = presentSemaphores[mSwapchain->GetCurrentFrameIndex()].get();
		mWindow->PollMessages();
	}

	return 0;
}