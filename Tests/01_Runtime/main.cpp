#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformMonitor.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Resource/Texture/TextureImporter.h>

#define INSTANCE_COUNT 11

using namespace Hollow;

struct Transform
{
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale;
};

struct ConstantBuffer
{
	MatrixSIMD Model[INSTANCE_COUNT];
	MatrixSIMD View;
	MatrixSIMD Projection;
};

struct Vertex
{
	Vector3f Position;
	Vector3f Color;
	Vector2f TexCoord;
};

const ArrayList<Vertex> vertices =
{
	{{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
};

const ArrayList<uint32> indices =
{
	3, 1, 0,
	3, 2, 1,

	4, 5, 7,
	5, 6, 7,

	11, 9, 8,
	11, 10, 9,

	12, 13, 15,
	13, 14, 15,

	16, 17, 19,
	17, 18, 19,

	23, 21, 20,
	23, 22, 21
};

ArrayList<Transform> InstanceTransforms(INSTANCE_COUNT);

ConstantBuffer MVPData = {
		{},
		XMMatrixLookAtLH({0, 0, -5}, {0, 0 ,0}, {0, 1, 0}),
		XMMatrixPerspectiveFovLH(XMConvertToRadians(80), static_cast<float>(1920.f / 1080.f), 0.01f, 1000.f)
};

void UpdateTransforms()
{
	for (byte i = 0; i < INSTANCE_COUNT; i++)
	{
		InstanceTransforms[i].Rotation.x += i * 0.05f + 0.1f;
		InstanceTransforms[i].Rotation.y += i * 0.05f + 0.1f;
		InstanceTransforms[i].Rotation.z += i * 0.05f + 0.1f;

		MVPData.Model[i] = XMMatrixScaling(InstanceTransforms[i].Scale.x, InstanceTransforms[i].Scale.y, InstanceTransforms[i].Scale.z) *
			XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransforms[i].Rotation.y),
				XMConvertToRadians(InstanceTransforms[i].Rotation.z),
				XMConvertToRadians(InstanceTransforms[i].Rotation.x)) *
			XMMatrixTranslation(InstanceTransforms[i].Position.x, InstanceTransforms[i].Position.y, InstanceTransforms[i].Position.z);
	}
}

int main(int argC, char** argV)
{
	// Initialize the platform API
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	// Initialize the transforms
	float depth = 1.0f;
	for (byte i = 0; i < INSTANCE_COUNT; i++)
	{
		int centerIndex = (INSTANCE_COUNT - 1) / 2;
		int startPos = -centerIndex;

		depth *= -1.0f;

		InstanceTransforms[i].Position = { static_cast<float>(startPos + i), 0.0f, static_cast<float>(centerIndex / 2) * depth };
		InstanceTransforms[i].Rotation = { 0.0f, 0.0f, 0.0f };
		InstanceTransforms[i].Scale = { 1.0f, 1.0f, 1.0f };

		MVPData.Model[i] = XMMatrixScaling(InstanceTransforms[i].Scale.x, InstanceTransforms[i].Scale.y, InstanceTransforms[i].Scale.z) *
			XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransforms[i].Rotation.y),
				XMConvertToRadians(InstanceTransforms[i].Rotation.z),
				XMConvertToRadians(InstanceTransforms[i].Rotation.x)) *
			XMMatrixTranslation(InstanceTransforms[i].Position.x, InstanceTransforms[i].Position.y, InstanceTransforms[i].Position.z);
	}

	auto texture = TextureImporter::ImportTexture(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/01_Runtime/Resources/TestTexture.png");

	// Get the primary monitor
	auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1920, 1080 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = WindowMode::Borderless;
	desc.pMonitor = primaryMonitor;

	auto mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);
	mWindow->Show();

	// Create a graphics instance
	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.ApplicationName = "Hollow Engine";
	instanceDesc.InstanceName = "Hollow Engine";
	instanceDesc.EnabledExtensions = {};
	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);

	// Create a graphics device
	GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Instance = mGraphicsInstance;
	deviceDesc.GraphicsQueueCount = 1;
	deviceDesc.ComputeQueueCount = 1;
	deviceDesc.TransferQueueCount = 1;
	auto mDevice = mGraphicsInstance->CreateGraphicsDevice(deviceDesc);

	// Create a graphics queue for swapchain presentation
	GraphicsQueueDesc queueDesc = {};
	queueDesc.QueueType = GraphicsQueueType::Graphics;
	auto mPresentQueue = mDevice->CreateQueue(queueDesc);

	// Create a swapchain
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = mWindow->GetWindowResolution();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;
	swapchainDesc.pQueue = mPresentQueue;

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	// ----------------- CREATE BUNDLE MEMORY -----------------

	// Create bundle memory for pre-allocation
	GraphicsMemoryDesc hostMemoryDesc = {};
	hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
	hostMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);

	auto mHostMemory = mDevice->CreateGraphicsMemory(hostMemoryDesc);

	GraphicsMemoryDesc deviceMemoryDesc = {};
	deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
	deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(1024);

	auto mDeviceMemory = mDevice->CreateGraphicsMemory(deviceMemoryDesc);

	// ----------------- CREATE SHADERS -----------------

	MemoryBuffer vBuffer = {};
	SharedPtr<MemoryOwnedBuffer> vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/01_Runtime/Shaders/testVertex.vert", vBuffer);
	ShaderDesc vertexShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(vBuffer, "main", ShaderStage::Vertex, ShaderLanguage::GLSL, vShaderCode, errorMessage))
	{
		vertexShaderDesc.ShaderName = "TestVertexShader";
		vertexShaderDesc.Stage = ShaderStage::Vertex;
		vertexShaderDesc.EntryPoint = "main";
		vertexShaderDesc.Language = ShaderLanguage::GLSL;
		vertexShaderDesc.ShaderCode = vShaderCode;
	}

	auto mVertexShader = mDevice->CreateShader(vertexShaderDesc);

	MemoryBuffer fBuffer = {};
	SharedPtr<MemoryOwnedBuffer> fShaderCode;
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/01_Runtime/Shaders/testFrag.frag", fBuffer);
	ShaderDesc fragmentShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(fBuffer, "main", ShaderStage::Fragment, ShaderLanguage::GLSL, fShaderCode, errorMessage))
	{
		fragmentShaderDesc.Stage = ShaderStage::Fragment;
		fragmentShaderDesc.EntryPoint = "main";
		fragmentShaderDesc.Language = ShaderLanguage::GLSL;
		fragmentShaderDesc.ShaderCode = fShaderCode;
	}

	auto mFragmentShader = mDevice->CreateShader(fragmentShaderDesc);

	// ----------------- CREATE FENCE -----------------

	FenceDesc fenceDesc = {};
	fenceDesc.bSignalled = false;

	auto mFence = mDevice->CreateSyncFence(fenceDesc);

	// ----------------- CREATE COMMAND POOL -----------------

	CommandPoolDesc commandPoolDesc = {};
	commandPoolDesc.PoolType = CommandPoolType::Graphics;

	auto mCommandPool = mDevice->CreateCommandPool(commandPoolDesc);

	// ----------------- CREATE COMMAND BUFFER -----------------

	CommandBufferDesc commandBufferDesc = {};
	commandBufferDesc.pOwnerPool = mCommandPool;

	auto mCommandBuffer = mDevice->CreateCommandBuffer(commandBufferDesc);

	// ----------------- CREATE DEPTH TEXTURE -----------------

	TextureDesc depthTextureDesc = {};
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.ImageFormat = TextureFormat::D32_Float_S8_UInt;
	depthTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1};
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.SampleCount = TextureSampleCount::Sample1;
	depthTextureDesc.Type = TextureType::Texture2D;
	depthTextureDesc.Usage = TextureUsage::DepthStencilAttachment;
	depthTextureDesc.pMemory = mDeviceMemory;

	auto mDepthTexture = mDevice->CreateTexture(depthTextureDesc);

	TextureBufferDesc depthTextureViewDesc = {};
	depthTextureViewDesc.ArrayLayer = 0;
	depthTextureViewDesc.AspectFlags = TextureAspectFlags::DepthAspect;
	depthTextureViewDesc.MipLevel = 0;
	depthTextureViewDesc.pTexture = mDepthTexture;

	auto mDepthTextureView = mDevice->CreateTextureBuffer(depthTextureViewDesc);

	// ----------------- UPDATE DEPTH TEXTURE -----------------

	mCommandBuffer->BeginRecording();

	TextureBarrierUpdateDesc depthTextureBarrier = {};
	depthTextureBarrier.MipIndex = 0;
	depthTextureBarrier.ArrayIndex = 0;
	depthTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
	depthTextureBarrier.OldLayout = TextureMemoryLayout::Unknown;
	depthTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
	depthTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

	depthTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::DepthStencilAttachmentWrite;
	depthTextureBarrier.NewLayout = TextureMemoryLayout::DepthStencilAttachment;
	depthTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	depthTextureBarrier.DestinationStageFlags = PipelineStageFlags::EarlyFragmentTests;

	depthTextureBarrier.AspectMask = TextureAspectFlags::DepthAspect | TextureAspectFlags::StencilAspect;

	mCommandBuffer->SetTextureBarrier(mDepthTexture, depthTextureBarrier);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(mPresentQueue, &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

	mDevice->WaitForFence(&mFence, 1);
	mDevice->ResetFences(&mFence, 1);

	// ----------------- CREATE RENDERPASS -----------------

	ArrayList<SharedPtr<RenderPass>> mRenderPasses;

	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		RenderPassDesc renderPassDesc = {};

		RenderPassAttachmentDesc colorAttachment = {};
		colorAttachment.Format = swapchainDesc.SwapchainImageFormat;
		colorAttachment.SampleCount = TextureSampleCount::Sample1;
		colorAttachment.LoadOperation = RenderPassLoadOperation::Clear;
		colorAttachment.StoreOperation = RenderPassStoreOperation::Store;
		colorAttachment.StencilLoadOperation = RenderPassLoadOperation::Ignore;
		colorAttachment.StencilStoreOperation = RenderPassStoreOperation::Ignore;
		colorAttachment.InputLayout = TextureMemoryLayout::Unknown;
		colorAttachment.OutputLayout = TextureMemoryLayout::Present;
		colorAttachment.ArrayLevel = 0;
		colorAttachment.MipLevel = 0;
		colorAttachment.pTextureBuffer = mSwapchain->GetImageViews()[i];

		renderPassDesc.Attachments.push_back(colorAttachment);

		RenderPassAttachmentDesc depthAttachment = {};
		depthAttachment.Format = depthTextureDesc.ImageFormat;
		depthAttachment.SampleCount = depthTextureDesc.SampleCount;
		depthAttachment.LoadOperation = RenderPassLoadOperation::Clear;
		depthAttachment.StoreOperation = RenderPassStoreOperation::Store;
		depthAttachment.StencilLoadOperation = RenderPassLoadOperation::Ignore;
		depthAttachment.StencilStoreOperation = RenderPassStoreOperation::Ignore;
		depthAttachment.InputLayout = TextureMemoryLayout::DepthStencilAttachment;
		depthAttachment.OutputLayout = TextureMemoryLayout::DepthStencilAttachment;
		depthAttachment.ArrayLevel = 0;
		depthAttachment.MipLevel = 0;
		depthAttachment.pTextureBuffer = mDepthTextureView;

		renderPassDesc.TargetSize = mSwapchain->GetImageSize();
		renderPassDesc.DepthStencilAttachment = depthAttachment;

		auto renderPass = mDevice->CreateRenderPass(renderPassDesc);

		mRenderPasses.push_back(renderPass);
	}

	// ----------------- CREATE DESCRIPTORS -----------------

	// Descriptor Set Layout
	DescriptorLayoutDesc descriptorLayoutDesc = {};
	descriptorLayoutDesc.Entries = {
		{0, DescriptorType::UniformBuffer, ShaderStage::Vertex},
		{1, DescriptorType::Sampler, ShaderStage::Fragment},
		{2, DescriptorType::SampledImage, ShaderStage::Fragment}
	};

	auto mDescriptorLayout = mDevice->CreateDescriptorLayout(descriptorLayoutDesc);

	// Descriptor Pool
	DescriptorPoolDesc descriptorPoolDesc = {};
	descriptorPoolDesc.Type = GraphicsMemoryType::HostVisible;
	descriptorPoolDesc.PoolSizes = {
		{DescriptorType::UniformBuffer, 1},
		{DescriptorType::Sampler, 1},
		{DescriptorType::SampledImage, 1}
	};

	auto mDescriptorPool = mDevice->CreateDescriptorPool(descriptorPoolDesc);

	// Descriptor Set
	DescriptorSetDesc descriptorSetDesc = {};
	descriptorSetDesc.pLayout = mDescriptorLayout;
	descriptorSetDesc.pOwnerPool = mDescriptorPool;

	auto mDescriptorSet = mDevice->CreateDescriptorSet(descriptorSetDesc);

	// ----------------- CREATE PIPELINE -----------------

	// BlendStateAttachment	
	BlendStateAttachment blendAttachment = {};
	blendAttachment.bEnabled = true;
	blendAttachment.ColorOperation = BlendOperation::Add;
	blendAttachment.AlphaOperation = BlendOperation::Add;
	blendAttachment.SourceColorFactor = BlendFactor::One;
	blendAttachment.DestinationColorFactor = BlendFactor::Zero;
	blendAttachment.SourceAlphaFactor = BlendFactor::One;
	blendAttachment.DestinationAlphaFactor = BlendFactor::Zero;
	blendAttachment.WriteMask = BlendColorWriteMask::All;

	// Create BlendState
	BlendStateDesc blendState = {};
	blendState.bLogicOperationEnabled = true;
	blendState.LogicOperation = LogicOperation::Copy;
	blendState.Attachments = { blendAttachment };

	// Create DepthStencilState
	DepthStencilStateDesc depthStencilState = {};
	depthStencilState.bDepthTestEnabled = true;
	depthStencilState.bDepthWriteEnabled = true;
	depthStencilState.bStencilTestEnabled = false;
	depthStencilState.DepthTestOperation = CompareOperation::Less;
	depthStencilState.StencilBackFace = {};
	depthStencilState.StencilFrontFace = {};

	// Position InputElement
	InputElement inputElement1 = {};
	inputElement1.Format = TextureFormat::RGB32_Float;
	inputElement1.Semantic = InputElementSemantic::Position;

	// Color InputElement
	InputElement inputElement2 = {};
	inputElement2.Format = TextureFormat::RGB32_Float;
	inputElement2.Semantic = InputElementSemantic::Color;

	// TexCoord InputElement
	InputElement inputElement3 = {};
	inputElement3.Format = TextureFormat::RG32_Float;
	inputElement3.Semantic = InputElementSemantic::TexCoord;

	// InputBinding
	InputBinding inputBinding = {};
	inputBinding.StepRate = InputBindingStepRate::Vertex;
	inputBinding.Elements = { inputElement1, inputElement2, inputElement3 };

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
	viewport.ViewportSize = mSwapchain->GetImageSize();

	// Create Scissor
	ScissorDesc scissor = {};
	scissor.OffsetSize = { 0, 0 };
	scissor.ScissorSize = mSwapchain->GetImageSize();

	ArrayList<SharedPtr<Pipeline>> mGraphicsPipelines;

	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		// Create Pipeline
		GraphicsPipelineDesc pipelineDesc = {};
		pipelineDesc.BlendState = blendState;
		pipelineDesc.DepthStencilState = depthStencilState;
		pipelineDesc.InputLayout = inputLayout;
		pipelineDesc.Multisample = multisample;
		pipelineDesc.RasterizerState = rasterizerState;
		pipelineDesc.ResourceLayout.ResourceLayouts = { mDescriptorLayout };
		pipelineDesc.GraphicsShaders = { mVertexShader, mFragmentShader };
		pipelineDesc.pRenderPass = mRenderPasses[i];
		pipelineDesc.Viewport = viewport;
		pipelineDesc.Scissor = scissor;
		pipelineDesc.SubpassIndex = 0;

		auto mPipeline = mDevice->CreateGraphicsPipeline(pipelineDesc);

		mGraphicsPipelines.push_back(mPipeline);
	}

	// ----------------- CREATE SAMPLER -----------------

	// Sampler
	SamplerDesc samplerDesc = {};
	samplerDesc.bAnisotropyEnabled = false;
	samplerDesc.bCompareEnabled = false;
	samplerDesc.BorderColor = SamplerBorderColor::TransparentBlack;
	samplerDesc.CompareOp = CompareOperation::Always;
	samplerDesc.MagFilter = SamplerFilter::Nearest;
	samplerDesc.MapMode = SamplerMapMode::Nearest;
	samplerDesc.MaxAnisotropy = 1.0f;
	samplerDesc.MaxLOD = 1.0f;
	samplerDesc.MinFilter = SamplerFilter::Nearest;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.UAddressMode = SamplerAddressMode::Repeat;
	samplerDesc.VAddressMode = SamplerAddressMode::Repeat;
	samplerDesc.WAddressMode = SamplerAddressMode::Repeat;

	auto mSampler = mDevice->CreateSampler(samplerDesc);

	// ----------------- CREATE OBJECT BUFFER -----------------

	GraphicsBufferDesc vertexBufferDesc = {};
	vertexBufferDesc.SubResourceCount = vertices.size();
	vertexBufferDesc.SubSizeInBytes = sizeof(Vertex);
	vertexBufferDesc.Usage = GraphicsBufferUsage::Vertex | GraphicsBufferUsage::TransferDestination;
	vertexBufferDesc.ShareMode = ShareMode::Exclusive;
	vertexBufferDesc.pMemory = mDeviceMemory;

	auto mVertexBuffer = mDevice->CreateGraphicsBuffer(vertexBufferDesc);

	GraphicsBufferDesc indexBufferDesc = {};
	indexBufferDesc.SubResourceCount = indices.size();
	indexBufferDesc.SubSizeInBytes = sizeof(uint32);
	indexBufferDesc.Usage = GraphicsBufferUsage::Index | GraphicsBufferUsage::TransferDestination;
	indexBufferDesc.ShareMode = ShareMode::Exclusive;
	indexBufferDesc.pMemory = mDeviceMemory;

	auto mIndexBuffer = mDevice->CreateGraphicsBuffer(indexBufferDesc);

	GraphicsBufferDesc uniformBufferDesc = {};
	uniformBufferDesc.SubResourceCount = 1;
	uniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	uniformBufferDesc.Usage = GraphicsBufferUsage::Uniform | GraphicsBufferUsage::TransferDestination;
	uniformBufferDesc.ShareMode = ShareMode::Exclusive;
	uniformBufferDesc.pMemory = mDeviceMemory;

	auto mUniformBuffer = mDevice->CreateGraphicsBuffer(uniformBufferDesc);

	TextureDesc textureDesc = {};
	textureDesc.ArraySize = 1;
	textureDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
	textureDesc.ImageSize = { static_cast<uint32>(texture->ImageSize.x), static_cast<uint32>(texture->ImageSize.y), 1 };
	textureDesc.MipLevels = 1;
	textureDesc.SampleCount = TextureSampleCount::Sample1;
	textureDesc.Type = TextureType::Texture2D;
	textureDesc.Usage = TextureUsage::TransferDestination | TextureUsage::Sampled;
	textureDesc.pMemory = mDeviceMemory;

	auto mTexture = mDevice->CreateTexture(textureDesc);

	TextureBufferDesc textureViewDesc = {};
	textureViewDesc.ArrayLayer = 0;
	textureViewDesc.AspectFlags = TextureAspectFlags::ColorAspect;
	textureViewDesc.MipLevel = 0;
	textureViewDesc.pTexture = mTexture;

	auto mTextureView = mDevice->CreateTextureBuffer(textureViewDesc);

	// ----------------- CREATE STAGING BUFFER -----------------

	GraphicsBufferDesc stagingVertexBufferDesc = {};
	stagingVertexBufferDesc.SubResourceCount = vertices.size();
	stagingVertexBufferDesc.SubSizeInBytes = sizeof(Vertex);
	stagingVertexBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingVertexBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingVertexBufferDesc.pMemory = mHostMemory;

	auto mStagingVertexBuffer = mDevice->CreateGraphicsBuffer(stagingVertexBufferDesc);

	GraphicsBufferDesc stagingIndexBufferDesc = {};
	stagingIndexBufferDesc.SubResourceCount = indices.size();
	stagingIndexBufferDesc.SubSizeInBytes = sizeof(uint32);
	stagingIndexBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingIndexBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingIndexBufferDesc.pMemory = mHostMemory;

	auto mStagingIndexBuffer = mDevice->CreateGraphicsBuffer(stagingIndexBufferDesc);

	GraphicsBufferDesc stagingUniformBufferDesc = {};
	stagingUniformBufferDesc.SubResourceCount = 1;
	stagingUniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	stagingUniformBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingUniformBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingUniformBufferDesc.pMemory = mHostMemory;

	auto mStagingUniformBuffer = mDevice->CreateGraphicsBuffer(stagingUniformBufferDesc);

	GraphicsBufferDesc textureStagingBufferDesc = {};
	textureStagingBufferDesc.ShareMode = ShareMode::Exclusive;
	textureStagingBufferDesc.SubResourceCount = 1;
	textureStagingBufferDesc.SubSizeInBytes = texture->ImageData.GetSize();
	textureStagingBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	textureStagingBufferDesc.pMemory = mHostMemory;

	auto mStagingTextureBuffer = mDevice->CreateGraphicsBuffer(textureStagingBufferDesc);

	// ----------------- UPDATE STAGING BUFFER -----------------

	uint64 vertexBufferSize = sizeof(Vertex) * vertices.size();
	uint64 indexBufferSize = sizeof(uint32) * indices.size();
	uint64 uniformBufferSize = sizeof(ConstantBuffer);

	BufferDataUpdateDesc vertexDataUpdateDesc;
	vertexDataUpdateDesc.Memory = { (void*)vertices.data(), vertexBufferSize };
	vertexDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingVertexBuffer, vertexDataUpdateDesc);

	BufferDataUpdateDesc indexDataUpdateDesc;
	indexDataUpdateDesc.Memory = { (void*)indices.data(), indexBufferSize };
	indexDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingIndexBuffer, indexDataUpdateDesc);

	BufferDataUpdateDesc uniformDataUpdateDesc;
	uniformDataUpdateDesc.Memory = { (void*)&MVPData, uniformBufferSize};
	uniformDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

	BufferDataUpdateDesc textureDataUpdateDesc = {};
	textureDataUpdateDesc.Memory = texture->ImageData;
	textureDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingTextureBuffer, textureDataUpdateDesc);

	// ----------------- COPY STAGING BUFFER TO OBJECT BUFFER -----------------

	mCommandBuffer->BeginRecording();
	BufferBufferCopyDesc vertexCopyDesc = {};
	vertexCopyDesc.DestinationOffset = 0;
	vertexCopyDesc.Size = vertexBufferSize;
	vertexCopyDesc.SourceOffset = 0;
	mCommandBuffer->CopyBufferToBuffer(mStagingVertexBuffer, mVertexBuffer, vertexCopyDesc);

	BufferBufferCopyDesc indexCopyDesc = {};
	indexCopyDesc.DestinationOffset = 0;
	indexCopyDesc.Size = indexBufferSize;
	indexCopyDesc.SourceOffset = 0;

	mCommandBuffer->CopyBufferToBuffer(mStagingIndexBuffer, mIndexBuffer, indexCopyDesc);

	BufferBufferCopyDesc uniformCopyDesc = {};
	uniformCopyDesc.DestinationOffset = 0;
	uniformCopyDesc.Size = uniformBufferSize;
	uniformCopyDesc.SourceOffset = 0;

	mCommandBuffer->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

	// First, make sure that texture is ready to be written.
	TextureBarrierUpdateDesc preTextureBarrier = {};
	preTextureBarrier.MipIndex = 0;
	preTextureBarrier.ArrayIndex = 0;
	preTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
	preTextureBarrier.OldLayout = TextureMemoryLayout::Unknown;
	preTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
	preTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

	preTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::TransferWrite;
	preTextureBarrier.NewLayout = TextureMemoryLayout::TransferDestination;
	preTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	preTextureBarrier.DestinationStageFlags = PipelineStageFlags::Transfer;

	preTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

	mCommandBuffer->SetTextureBarrier(mTexture, preTextureBarrier);

	BufferTextureCopyDesc textureCopyDesc = {};
	textureCopyDesc.BufferOffsetInBytes = 0;
	textureCopyDesc.TargetArrayIndex = 0;
	textureCopyDesc.TargetMipIndex = 0;
	textureCopyDesc.TextureOffset = { 0,0,0 };
	textureCopyDesc.TextureSize = textureDesc.ImageSize;
	mCommandBuffer->CopyBufferToTexture(mStagingTextureBuffer, mTexture, textureCopyDesc);

	TextureBarrierUpdateDesc postTextureBarrier = {};
	postTextureBarrier.MipIndex = 0;
	postTextureBarrier.ArrayIndex = 0;
	postTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::TransferWrite;
	postTextureBarrier.OldLayout = TextureMemoryLayout::TransferDestination;
	postTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
	postTextureBarrier.SourceStageFlags = PipelineStageFlags::Transfer;

	postTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ShaderRead;
	postTextureBarrier.NewLayout = TextureMemoryLayout::ShaderReadOnly;
	postTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	postTextureBarrier.DestinationStageFlags = PipelineStageFlags::FragmentShader;

	postTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

	mCommandBuffer->SetTextureBarrier(mTexture, postTextureBarrier);

	DescriptorSetUpdateDesc descriptorUpdateDesc = {};
	descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
	descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
	descriptorUpdateDesc.Entries.push_back({ mTextureView, DescriptorType::SampledImage, 1, 0, 0, 2 });

	mDevice->UpdateDescriptorSet(mDescriptorSet, descriptorUpdateDesc);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(mPresentQueue, &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

	mDevice->WaitForFence(&mFence, 1);
	mDevice->ResetFences(&mFence, 1);

	// ----------------- RENDER LOOP -----------------

	float elapsedTime = 0.0f;

	while (mWindow->IsVisible())
	{
		UpdateTransforms();

		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = { &MVPData, sizeof(ConstantBuffer) };
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		mWindow->PollEvents();

		mCommandBuffer->BeginRecording();

		BufferBufferCopyDesc constantCopyDesc = {};
		constantCopyDesc.DestinationOffset = 0;
		constantCopyDesc.Size = constantDataUpdateDesc.Memory.GetSize();
		constantCopyDesc.SourceOffset = 0;
		mCommandBuffer->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, constantCopyDesc);

		mCommandBuffer->BeginRenderPass(mRenderPasses.data(), { 0.6f, 0.3f, 0.5f, 0.0f });

		mCommandBuffer->BindPipeline(mGraphicsPipelines.data());
		mCommandBuffer->SetViewports(&viewport, 1);
		mCommandBuffer->SetScissors(&scissor, 1);

		mCommandBuffer->BindVertexBuffers(&mVertexBuffer, 1);
		mCommandBuffer->BindIndexBuffer(mIndexBuffer, GraphicsIndexType::Index32);

		mDevice->UpdateDescriptorSet(mDescriptorSet, descriptorUpdateDesc);
		mCommandBuffer->BindDescriptors(&mDescriptorSet, 1);

		mCommandBuffer->DrawIndexed(indices.size(), 0, 0, 0, INSTANCE_COUNT);

		mCommandBuffer->EndRenderPass();
		mCommandBuffer->EndRecording();

		mDevice->SubmitToQueue(mPresentQueue, &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);
		mDevice->WaitForFence(&mFence, 1);
		mDevice->ResetFences(&mFence, 1);

		mSwapchain->Present(nullptr, 0);
	}

	mDevice->WaitForIdle();

	return 0;
}
