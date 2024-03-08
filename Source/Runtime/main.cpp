#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>

using namespace Hollow;

struct Vertex
{
	Vector2f Position;
	Vector3f Color;
};

const Array<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	{{-0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
	{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
	{{ 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}

};

const Array<uint32> indices =
{
	0, 1, 3,
	1, 2, 3
};

int main()
{
	// Create a window
	WindowDesc winDesc = {};
	winDesc.Windowed = false;
	winDesc.WindowSize = { 1920, 1080 };
	winDesc.WindowTitle = "Test Application";
	winDesc.WindowPosition = { (2560 - 1920) / 2, (1440 - 1080) / 2 };

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

	// ----------------- CREATE FENCE -----------------

	FenceDesc fenceDesc = {};
	fenceDesc.bSignaled = false;

	auto mFence = mDevice->CreateFence(fenceDesc).get();

	// ----------------- CREATE RENDERPASS -----------------

	RenderPassDesc renderPassDesc = {};
	renderPassDesc.TargetRenderSize = winDesc.WindowSize;
	renderPassDesc.Format = swapchainDesc.SwapchainImageFormat;
	renderPassDesc.SampleCount = TextureSampleCount::Sample1;
	renderPassDesc.ColorLoadOperation = RenderPassLoadOperation::Clear;
	renderPassDesc.ColorStoreOperation = RenderPassStoreOperation::Store;
	renderPassDesc.StencilLoadOperation = RenderPassLoadOperation::Ignore;
	renderPassDesc.StencilStoreOperation = RenderPassStoreOperation::Ignore;
	renderPassDesc.InputLayout = TextureMemoryLayout::Unknown;
	renderPassDesc.OutputLayout = TextureMemoryLayout::Present;
	renderPassDesc.ArrayLevel = 1;
	renderPassDesc.MipLevel = 1;
	for(auto& view : mSwapchain->GetImageViews())
		renderPassDesc.Views.push_back(view.get());

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
	inputElement2.Format = TextureFormat::RGB32_Float;
	inputElement2.Semantic = InputElementSemantic::Color;

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

	// ----------------- CREATE COMMAND POOL -----------------

	CommandPoolDesc commandPoolDesc = {};
	commandPoolDesc.PoolType = CommandPoolType::Graphics;

	auto mCommandPool = mDevice->CreateCommandPool(commandPoolDesc);

	// ----------------- CREATE COMMAND BUFFER -----------------

	CommandBufferDesc commandBufferDesc = {};
	commandBufferDesc.pOwnerPool = mCommandPool.get();

	auto mCommandBuffer = mDevice->CreateCommandBuffer(commandBufferDesc).get();

	// ----------------- CREATE VERTEX & INDEX BUFFERS -----------------

	// Vertex Buffer
	GraphicsBufferDesc vertexBufferDesc = {};
	vertexBufferDesc.MemoryType = GraphicsMemoryType::Host;
	vertexBufferDesc.ShareMode = ShareMode::Exclusive;
	vertexBufferDesc.SizeInBytes = sizeof(Vertex) * vertices.size();
	vertexBufferDesc.Usage = GraphicsBufferUsage::TransferDestination | GraphicsBufferUsage::Vertex;

	GraphicsBuffer* mVertexBuffer = mDevice->CreateGraphicsBuffer(vertexBufferDesc).get();
	mVertexBuffer->AllocateMemory();

	// Index Buffer
	GraphicsBufferDesc indexBufferDesc = {};
	indexBufferDesc.MemoryType = GraphicsMemoryType::Host;
	indexBufferDesc.ShareMode = ShareMode::Exclusive;
	indexBufferDesc.SizeInBytes = sizeof(uint32) * indices.size();
	indexBufferDesc.Usage = GraphicsBufferUsage::TransferDestination | GraphicsBufferUsage::Index;

	GraphicsBuffer* mIndexBuffer = mDevice->CreateGraphicsBuffer(indexBufferDesc).get();
	mIndexBuffer->AllocateMemory();

	// ----------------- CREATE RELATED STAGING BUFFERS -----------------

	// Vertex Staging Buffer
	GraphicsBufferDesc vertexStagingBufferDesc = {};
	vertexStagingBufferDesc.MemoryType = GraphicsMemoryType::Host;
	vertexStagingBufferDesc.ShareMode = ShareMode::Exclusive;
	vertexStagingBufferDesc.SizeInBytes = sizeof(Vertex) * vertices.size();
	vertexStagingBufferDesc.Usage = GraphicsBufferUsage::TransferSource;

	auto mVertexStagingBuffer = mDevice->CreateGraphicsBuffer(vertexStagingBufferDesc);
	mVertexStagingBuffer->AllocateMemory();

	// Index Staging Buffer
	GraphicsBufferDesc indexStagingBufferDesc = {};
	indexStagingBufferDesc.MemoryType = GraphicsMemoryType::Host;
	indexStagingBufferDesc.ShareMode = ShareMode::Exclusive;
	indexStagingBufferDesc.SizeInBytes = sizeof(uint32) * indices.size();
	indexStagingBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
		
	auto mIndexStagingBuffer = mDevice->CreateGraphicsBuffer(indexStagingBufferDesc);
	mIndexStagingBuffer->AllocateMemory();

	// ----------------- STAGING TO REAL -----------------
	
	// Copy vertex data to staging buffer
	BufferDataUpdateDesc vertexDataUpdateDesc = {};
	vertexDataUpdateDesc.pData = (void*)vertices.data();
	vertexDataUpdateDesc.SizeInBytes = sizeof(Vertex) * vertices.size();

	mDevice->UpdateBufferData(mVertexStagingBuffer.get(), vertexDataUpdateDesc);

	// Copy index data to staging buffer
	BufferDataUpdateDesc indexDataUpdateDesc = {};
	indexDataUpdateDesc.pData = (void*)indices.data();
	indexDataUpdateDesc.SizeInBytes = sizeof(uint32) * indices.size();

	mDevice->UpdateBufferData(mIndexStagingBuffer.get(), indexDataUpdateDesc);

	// Record command buffer
	
	mCommandBuffer->BeginRecording();

	// Copy vertex data from staging buffer to vertex buffer
	BufferBufferCopyDesc vertexCopyDesc = {};
	vertexCopyDesc.DestinationOffset = 0;
	vertexCopyDesc.Size = vertexDataUpdateDesc.SizeInBytes;
	vertexCopyDesc.SourceOffset = 0;
	mCommandBuffer->CopyBufferToBuffer(mVertexStagingBuffer.get(), mVertexBuffer, vertexCopyDesc);

	// Copy index data from staging buffer to index buffer
	BufferBufferCopyDesc indexCopyDesc = {};
	indexCopyDesc.DestinationOffset = 0;
	indexCopyDesc.Size = indexDataUpdateDesc.SizeInBytes;
	indexCopyDesc.SourceOffset = 0;
	mCommandBuffer->CopyBufferToBuffer(mIndexStagingBuffer.get(), mIndexBuffer, indexCopyDesc);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(mPresentQueue.get(), &mCommandBuffer, 1, nullptr, 0, nullptr, 0, mFence);

	mDevice->WaitForFence(&mFence, 1);
	mDevice->ResetFences(&mFence, 1);

	// ----------------- RENDER LOOP -----------------

	CORE_LOG(HE_VERBOSE, "Test Application", "Rendering started!");

	while (!mWindow->ShouldClose())
	{
		mWindow->PollMessages();

		mCommandBuffer->BeginRecording();
		mCommandBuffer->BeginRenderPass(mRenderPass.get());

		mCommandBuffer->BindPipeline(mPipeline.get());
		mCommandBuffer->SetViewports(&viewport, 1);
		mCommandBuffer->SetScissors(&scissor, 1);
		
		mCommandBuffer->BindVertexBuffers(&mVertexBuffer, 1);
		mCommandBuffer->BindIndexBuffer(mIndexBuffer, GraphicsIndexType::Index32);
		mCommandBuffer->DrawIndexed(indices.size(), 0, 0, 0, 1);
		
		mCommandBuffer->EndRenderPass();
		mCommandBuffer->EndRecording();
		
		mDevice->SubmitToQueue(mPresentQueue.get(), &mCommandBuffer, 1, nullptr, 0, nullptr, 0, mFence);
		mDevice->WaitForFence(&mFence, 1);
		mDevice->ResetFences(&mFence, 1);

		mSwapchain->Present(nullptr, 0);
	}

	mDevice->WaitForIdle();

	mDevice->OnShutdown();
	mGraphicsInstance->OnShutdown();
	mWindow->OnShutdown();

	return 0;
}