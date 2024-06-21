#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
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
#include <Runtime/Resource/Importer/ResourceImporter.h>

#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Resource/Mesh/MeshResource.h>

#define INSTANCE_COUNT 81
#define MSAA_SAMPLES 8
using namespace Hollow;

struct Transform
{
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale;
};

struct ConstantBuffer
{
	Matrix4f Model[INSTANCE_COUNT];
};

struct PushConstantData
{
	Matrix4f View;
	Matrix4f Projection;
};

struct Vertex
{
	Vector3f Position;
	Vector3f Color;
	Vector2f TexCoord;
};

const DArray<Vertex> vertices =
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

const DArray<u32> indices =
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

DArray<Transform> InstanceTransforms(INSTANCE_COUNT);

ConstantBuffer MVPData = {
	{}
};

PushConstantData PushConstants =
{
	.View = Math::lookAtLH(Vec3f(0, 0, -7), Vec3f(0, 0 ,0), Vec3f(0, 1, 0)),
	.Projection = Math::perspectiveLH(Math::radians(70.f), (1300.f / 1300.f), 0.01f, 1000.f)
};

void UpdateTransforms()
{
	for (byte i = 0; i < INSTANCE_COUNT; i++)
	{
		InstanceTransforms[i].Rotation.x += Math::radians(0.01f);
		if (i % 2 == 0)
			InstanceTransforms[i].Rotation.z += Math::radians(0.02f);
		else
			InstanceTransforms[i].Rotation.z -= Math::radians(0.014f);

		InstanceTransforms[i].Rotation.y += Math::radians(0.03f);

		Mat4f scaleMatrix = Math::scale(Mat4f(1.0f), InstanceTransforms[i].Scale);
		Mat4f rotationMatrix = Math::rotate(Mat4f(1.0f), InstanceTransforms[i].Rotation.x, Vec3f(1, 0, 0)) *
			Math::rotate(Mat4f(1.0f), InstanceTransforms[i].Rotation.y, Vec3f(0, 1, 0)) *
			Math::rotate(Mat4f(1.0f), InstanceTransforms[i].Rotation.z, Vec3f(0, 0, 1));
		Mat4f translationMatrix = Math::translate(Mat4f(1.0f), InstanceTransforms[i].Position);

		MVPData.Model[i] = translationMatrix * rotationMatrix * scaleMatrix;
	}
}

int main(int argC, char** argV)
{
	// Initialize the platform API
	PlatformAPI::GetAPI().InitializeArguments(argC, argV);

#pragma region Object Cacophony
	// make them start at the right up corner and go left
	i32 sideLength = i32(sqrt(INSTANCE_COUNT));
	i32 numCols = sideLength;

	f32 initialX = 1 - numCols;
	f32 initialY = -numCols;

	// Initialize the transforms
	for (byte i = 0; i < i32(sqrt(INSTANCE_COUNT)); i++)
	{
		initialY += 1;

		for (i32 j = 0; j < i32(sqrt(INSTANCE_COUNT)); j++)
		{
			f32 xPos = initialX + j;

			InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Position = { xPos + j, initialY + i, (f32)sideLength - 1 };
			InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Rotation = { 0, 0, 0 };
			InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Scale = { 1, 1, 1 };

			Mat4f scaleMatrix = Math::scale(Mat4f(1.0f), InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Scale);
			Mat4f rotationMatrix = Math::rotate(Mat4f(1.0f), InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Rotation.x, Vec3f(1, 0, 0)) *
				Math::rotate(Mat4f(1.0f), InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Rotation.y, Vec3f(0, 1, 0)) *
				Math::rotate(Mat4f(1.0f), InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Rotation.z, Vec3f(0, 0, 1));
			Mat4f translationMatrix = Math::translate(Mat4f(1.0f), InstanceTransforms[i * i32(sqrt(INSTANCE_COUNT)) + j].Position);

			MVPData.Model[i * i32(sqrt(INSTANCE_COUNT)) + j] = translationMatrix * rotationMatrix * scaleMatrix;
		}
	}

	auto texture = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/01_Runtime/Resources/TestTexture.png");
#pragma endregion

#pragma region Window and Graphics Initialization
	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1300, 1300 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow - Instancing";
	desc.WindowMode = WindowMode::Windowed;

	auto mWindow = WindowManager::GetAPI().InitializeWindow(desc);

	// Create a graphics instance
	GraphicsInstanceDesc instanceDesc = {};
	instanceDesc.API = GraphicsAPI::Vulkan;
	instanceDesc.ApplicationName = "Hollow Engine";
	instanceDesc.InstanceName = "Hollow Engine";
	auto mGraphicsInstance = GraphicsInstance::CreateInstance(instanceDesc);

	// Create a graphics device
	GraphicsDeviceDesc deviceDesc = {};
	deviceDesc.Instance = mGraphicsInstance;
	deviceDesc.GraphicsQueueCount = 1;
	deviceDesc.ComputeQueueCount = 1;
	deviceDesc.TransferQueueCount = 1;
	auto mDevice = mGraphicsInstance->CreateGraphicsDevice(deviceDesc);

	// Create a swapchain
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = mWindow->GetWindowResolution();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;
	swapchainDesc.pQueue = GraphicsManager::GetAPI().GetDefaultPresentQueue();

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);
#pragma endregion

#pragma region Pre-Allocated Memory
	// ----------------- CREATE BUNDLE MEMORY -----------------

	// Create bundle memory for pre-allocation
	GraphicsMemoryDesc hostMemoryDesc = {};
	hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
	hostMemoryDesc.SizeInBytes = MB_TO_BYTE(5);

	auto mHostMemory = mDevice->CreateGraphicsMemory(hostMemoryDesc);

	GraphicsMemoryDesc deviceMemoryDesc = {};
	deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
	deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(109);

	auto mDeviceMemory = mDevice->CreateGraphicsMemory(deviceMemoryDesc);
#pragma endregion

#pragma region Shader Compilation
	// ----------------- CREATE SHADERS -----------------

	MemoryBuffer vBuffer = {};
	MemoryOwnedBuffer vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/01_Runtime/Shaders/testVertex.vert", vBuffer);
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
	MemoryOwnedBuffer fShaderCode;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/01_Runtime/Shaders/testFrag.frag", fBuffer);
	ShaderDesc fragmentShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(fBuffer, "main", ShaderStage::Fragment, ShaderLanguage::GLSL, fShaderCode, errorMessage))
	{
		fragmentShaderDesc.Stage = ShaderStage::Fragment;
		fragmentShaderDesc.EntryPoint = "main";
		fragmentShaderDesc.Language = ShaderLanguage::GLSL;
		fragmentShaderDesc.ShaderCode = fShaderCode;
	}

	auto mFragmentShader = mDevice->CreateShader(fragmentShaderDesc);
#pragma endregion

#pragma region Fence and Command Buffer Initialization
	// ----------------- CREATE FENCE -----------------

	DArray<SharedPtr<Fence>> mRuntimeFences;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		mRuntimeFences.push_back(mDevice->CreateSyncFence({ false }));
	}

	auto mCompileFence = mDevice->CreateSyncFence({ false });

	// ----------------- CREATE COMMAND POOL -----------------

	CommandPoolDesc commandPoolDesc = {};
	commandPoolDesc.PoolType = CommandPoolType::Graphics;

	auto mCommandPool = mDevice->CreateCommandPool(commandPoolDesc);

	// ----------------- CREATE COMMAND BUFFER -----------------

	CommandBufferDesc commandBufferDesc = {};
	commandBufferDesc.pOwnerPool = mCommandPool;

	auto mCommandBuffer = mDevice->CreateCommandBuffer(commandBufferDesc);

	DArray<SharedPtr<CommandBuffer>> mCommandBuffers;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		mCommandBuffers.push_back(mDevice->CreateCommandBuffer(commandBufferDesc));
	}
#pragma endregion

#pragma region Depth and Multisample Textures
	// ----------------- CREATE DEPTH TEXTURE -----------------

	TextureDesc depthTextureDesc = {};
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.ImageFormat = TextureFormat::D32_Float;
	depthTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1 };
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.SampleCount = static_cast<TextureSampleCount>(MSAA_SAMPLES);
	depthTextureDesc.Type = TextureType::Texture2DMS;
	depthTextureDesc.Usage = TextureUsage::DepthStencilAttachment;
	depthTextureDesc.pMemory = mDeviceMemory;

	auto mDepthTexture = mDevice->CreateTexture(depthTextureDesc);

	TextureBufferDesc depthTextureViewDesc = {};
	depthTextureViewDesc.ArrayLayer = 0;
	depthTextureViewDesc.AspectFlags = TextureAspectFlags::DepthAspect;
	depthTextureViewDesc.MipLevel = 0;
	depthTextureViewDesc.pTexture = mDepthTexture;

	auto mDepthTextureView = mDevice->CreateTextureBuffer(depthTextureViewDesc);

	// ----------------- CREATE MULTISAMPLE TEXTURES FOR SWAPCHAIN -----------------
	SharedPtr<Texture> mColorTexture;
	SharedPtr<TextureBuffer> mColorTextureView;

	TextureDesc colorTextureDesc = {};
	colorTextureDesc.ArraySize = 1;
	colorTextureDesc.ImageFormat = swapchainDesc.SwapchainImageFormat;
	colorTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1 };
	colorTextureDesc.MipLevels = 1;
	colorTextureDesc.SampleCount = static_cast<TextureSampleCount>(MSAA_SAMPLES);
	colorTextureDesc.Type = TextureType::Texture2DMS;
	colorTextureDesc.Usage = TextureUsage::ColorAttachment;
	colorTextureDesc.pMemory = mDeviceMemory;

	mColorTexture = mDevice->CreateTexture(colorTextureDesc);

	TextureBufferDesc colorTextureViewDesc = {};
	colorTextureViewDesc.ArrayLayer = 0;
	colorTextureViewDesc.AspectFlags = TextureAspectFlags::ColorAspect;
	colorTextureViewDesc.MipLevel = 0;
	colorTextureViewDesc.pTexture = mColorTexture;

	mColorTextureView = mDevice->CreateTextureBuffer(colorTextureViewDesc);

	// ----------------- UPDATE DEPTH TEXTURE -----------------

	mCommandBuffer->BeginRecording();

	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		auto colorBuffer = mSwapchain->GetImage(i);
		TextureBarrierUpdateDesc colorTextureBarrier = {};
		colorTextureBarrier.MipIndex = 0;
		colorTextureBarrier.ArrayIndex = 0;
		colorTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
		colorTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
		colorTextureBarrier.OldLayout = TextureMemoryLayout::Unknown;
		colorTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

		colorTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
		colorTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		colorTextureBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		colorTextureBarrier.NewLayout = TextureMemoryLayout::Present;

		colorTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffer->SetTextureBarrier(colorBuffer, colorTextureBarrier);
	}

	TextureBarrierUpdateDesc msaaTextureBarrier = {};
	msaaTextureBarrier.MipIndex = 0;
	msaaTextureBarrier.ArrayIndex = 0;
	msaaTextureBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::Unknown;
	msaaTextureBarrier.OldLayout = TextureMemoryLayout::Unknown;
	msaaTextureBarrier.SourceQueue = GraphicsQueueType::Graphics;
	msaaTextureBarrier.SourceStageFlags = PipelineStageFlags::TopOfPipe;

	msaaTextureBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
	msaaTextureBarrier.NewLayout = TextureMemoryLayout::ColorAttachment;
	msaaTextureBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	msaaTextureBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;

	msaaTextureBarrier.AspectMask = TextureAspectFlags::ColorAspect;

	mCommandBuffer->SetTextureBarrier(mColorTexture, msaaTextureBarrier);

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

	depthTextureBarrier.AspectMask = TextureAspectFlags::DepthAspect;

	mCommandBuffer->SetTextureBarrier(mDepthTexture, depthTextureBarrier);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);

#pragma endregion

#pragma region Descriptor Initialization
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
	descriptorPoolDesc.MaxSets = 10;
	descriptorPoolDesc.PoolSizes = {
		{DescriptorType::UniformBuffer, 1},
		{DescriptorType::Sampler, 1},
		{DescriptorType::SampledImage, 1},
		{DescriptorType::StorageImage, 1},
		{DescriptorType::StorageBuffer, 1},
		{DescriptorType::UniformTexelBuffer, 1},
		{DescriptorType::StorageTexelBuffer, 1},
		{DescriptorType::InputAttachment, 1}
	};

	auto mDescriptorPool = mDevice->CreateDescriptorPool(descriptorPoolDesc);

	// Descriptor Set
	DescriptorSetDesc descriptorSetDesc = {};
	descriptorSetDesc.pLayout = mDescriptorLayout;
	descriptorSetDesc.pOwnerPool = mDescriptorPool;

	auto mDescriptorSet = mDevice->CreateDescriptorSet(descriptorSetDesc);
#pragma endregion

#pragma region Pipeline Initialization
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
	multisample.bSampleShadingEnabled = true;
	multisample.Samples = static_cast<TextureSampleCount>(MSAA_SAMPLES);

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

	// Create PushConstantRange
	PushConstantRange range1 = {};
	range1.Offset = 0;
	range1.Size = sizeof(PushConstantData);
	range1.Stage = ShaderStage::Vertex;

	PushConstantDesc pushConstant = {};
	pushConstant.PushConstantRanges = { range1 };

	// Create Pipeline
	GraphicsPipelineDesc pipelineDesc = {};
	pipelineDesc.BlendState = blendState;
	pipelineDesc.DepthStencilState = depthStencilState;
	pipelineDesc.InputLayout = inputLayout;
	pipelineDesc.Multisample = multisample;
	pipelineDesc.RasterizerState = rasterizerState;
	pipelineDesc.ResourceLayout.ResourceLayouts = { mDescriptorLayout };
	pipelineDesc.GraphicsShaders = { mVertexShader, mFragmentShader };
	pipelineDesc.Viewport = viewport;
	pipelineDesc.Scissor = scissor;
	pipelineDesc.PushConstants = pushConstant;
	pipelineDesc.ColorAttachmentCount = 1;
	pipelineDesc.ColorAttachmentFormats = { TextureFormat::RGBA8_UNorm };
	pipelineDesc.DepthAttachmentFormat = TextureFormat::D32_Float;
	pipelineDesc.StencilAttachmentFormat = TextureFormat::Unknown;

	auto mPipeline = mDevice->CreateGraphicsPipeline(pipelineDesc);

#pragma endregion

#pragma region Sampler
	// ----------------- CREATE SAMPLER -----------------

	// Sampler
	SamplerDesc samplerDesc = {};
	samplerDesc.bAnisotropyEnabled = true;
	samplerDesc.bCompareEnabled = false;
	samplerDesc.BorderColor = SamplerBorderColor::TransparentBlack;
	samplerDesc.CompareOp = CompareOperation::Always;
	samplerDesc.MagFilter = SamplerFilter::Nearest;
	samplerDesc.MapMode = SamplerMapMode::Nearest;
	samplerDesc.MaxAnisotropy = 16.0f;
	samplerDesc.MaxLOD = 1.0f;
	samplerDesc.MinFilter = SamplerFilter::Nearest;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.UAddressMode = SamplerAddressMode::Repeat;
	samplerDesc.VAddressMode = SamplerAddressMode::Repeat;
	samplerDesc.WAddressMode = SamplerAddressMode::Repeat;

	auto mSampler = mDevice->CreateSampler(samplerDesc);
#pragma endregion

#pragma region Buffer Initialization
	// ----------------- CREATE OBJECT BUFFER -----------------

	SharedPtr<MeshResource> meshRes = MakeShared<MeshResource>();
	SharedPtr<TextureResource> texRes = MakeShared<TextureResource>();

	meshRes->ConnectMemory(mHostMemory, mDeviceMemory);
	texRes->ConnectMemory(mHostMemory, mDeviceMemory);

	meshRes->CreateMeshBuffers(sizeof(Vertex), vertices.size(), sizeof(u32), indices.size());

	TextureDesc textureDesc = {};
	textureDesc.ArraySize = 1;
	textureDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
	textureDesc.ImageSize = { static_cast<u32>(texture.ImageSize.x), static_cast<u32>(texture.ImageSize.y), 1 };
	textureDesc.MipLevels = 1;
	textureDesc.SampleCount = TextureSampleCount::Sample1;
	textureDesc.Type = TextureType::Texture2D;
	textureDesc.Usage = TextureUsage::TransferDestination | TextureUsage::Sampled;
	textureDesc.pMemory = mDeviceMemory;
	texRes->CreateTextureAndBuffer(textureDesc);

	GraphicsBufferDesc uniformBufferDesc = {};
	uniformBufferDesc.SubResourceCount = 1;
	uniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	uniformBufferDesc.Usage = GraphicsBufferUsage::Uniform | GraphicsBufferUsage::TransferDestination;
	uniformBufferDesc.ShareMode = ShareMode::Exclusive;
	uniformBufferDesc.pMemory = mDeviceMemory;

	auto mUniformBuffer = mDevice->CreateGraphicsBuffer(uniformBufferDesc);

	// ----------------- CREATE STAGING BUFFER -----------------

	GraphicsBufferDesc stagingUniformBufferDesc = {};
	stagingUniformBufferDesc.SubResourceCount = 1;
	stagingUniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	stagingUniformBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingUniformBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingUniformBufferDesc.pMemory = mHostMemory;

	auto mStagingUniformBuffer = mDevice->CreateGraphicsBuffer(stagingUniformBufferDesc);

	// ----------------- UPDATE STAGING BUFFER -----------------

	u64 vertexBufferSize = sizeof(Vertex) * vertices.size();
	u64 indexBufferSize = sizeof(u32) * indices.size();
	u64 uniformBufferSize = sizeof(ConstantBuffer);

	meshRes->UpdateVertexBuffer({ (void*)vertices.data(), vertexBufferSize }, 0);
	meshRes->UpdateIndexBuffer({ (void*)indices.data(), indexBufferSize }, 0);
	texRes->UpdateTextureAndBuffer(texture.ImageData, 0);

	BufferDataUpdateDesc uniformDataUpdateDesc = {};
	uniformDataUpdateDesc.Memory = MemoryBuffer((void*)&MVPData, uniformBufferSize);
	uniformDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

	// ----------------- COPY STAGING BUFFER TO OBJECT BUFFER -----------------

	mCommandBuffer->BeginRecording();

	BufferBufferCopyDesc uniformCopyDesc = {};
	uniformCopyDesc.DestinationOffset = 0;
	uniformCopyDesc.Size = uniformBufferSize;
	uniformCopyDesc.SourceOffset = 0;
	mCommandBuffer->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

	DescriptorSetUpdateDesc descriptorUpdateDesc = {};
	descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
	descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
	descriptorUpdateDesc.Entries.push_back({ texRes->GetTextureBuffer(), DescriptorType::SampledImage, 1, 0, 0, 2 });

	mDevice->UpdateDescriptorSet(mDescriptorSet, descriptorUpdateDesc);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);

#pragma endregion

#pragma region Main Loop
	mWindow->Show();
	while (!mWindow->IsClosed())
	{
		mSwapchain->AcquireNextImage();

		u32 imageIndex = mSwapchain->GetCurrentFrameIndex();
		auto perFence = mRuntimeFences[imageIndex];

		mCommandBuffers[imageIndex]->BeginRecording();

		UpdateTransforms();

		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = MemoryBuffer(&MVPData, sizeof(ConstantBuffer));
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		mWindow->PollEvents();

		BufferBufferCopyDesc constantCopyDesc = {};
		constantCopyDesc.DestinationOffset = 0;
		constantCopyDesc.Size = constantDataUpdateDesc.Memory.GetSize();
		constantCopyDesc.SourceOffset = 0;
		mCommandBuffers[imageIndex]->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, constantCopyDesc);
		mDevice->UpdateDescriptorSet(mDescriptorSet, descriptorUpdateDesc);

		TextureBarrierUpdateDesc preRenderBarrier = {};
		preRenderBarrier.ArrayIndex = 0;
		preRenderBarrier.MipIndex = 0;
		preRenderBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
		preRenderBarrier.SourceQueue = GraphicsQueueType::Graphics;
		preRenderBarrier.SourceStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		preRenderBarrier.OldLayout = TextureMemoryLayout::Present;

		preRenderBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
		preRenderBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		preRenderBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		preRenderBarrier.NewLayout = TextureMemoryLayout::ColorAttachment;

		preRenderBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffers[imageIndex]->SetTextureBarrier(mSwapchain->GetImage(imageIndex), preRenderBarrier);

		DynamicPassAttachmentDesc passColorAttachmentDesc = {};
		passColorAttachmentDesc.ImageBuffer = mColorTextureView;
		passColorAttachmentDesc.ResolveLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.ResolveBuffer = mSwapchain->GetImageView(imageIndex);
		passColorAttachmentDesc.ResolveMode = ResolveModeFlags::Average;
		passColorAttachmentDesc.TextureLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassAttachmentDesc passDepthAttachmentDesc = {};
		passDepthAttachmentDesc.ImageBuffer = mDepthTextureView;
		passDepthAttachmentDesc.TextureLayout = TextureMemoryLayout::DepthStencilAttachment;
		passDepthAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passDepthAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passDepthAttachmentDesc.ClearDepthStencil = { 1.0f, 0 };

		DynamicPassDesc passDesc = {};
		passDesc.ColorAttachments = { passColorAttachmentDesc };
		passDesc.DepthAttachment = passDepthAttachmentDesc;
		passDesc.layerCount = 1;
		passDesc.Extent = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y };
		passDesc.Offset = { 0, 0 };
		passDesc.viewMask = 0;

		mCommandBuffers[imageIndex]->BeginRendering(passDesc);

		mCommandBuffers[imageIndex]->BindPipeline(mPipeline);
		mCommandBuffers[imageIndex]->SetViewports(&viewport, 1);
		mCommandBuffers[imageIndex]->SetScissors(&scissor, 1);
		mCommandBuffers[imageIndex]->BindVertexBuffers(&meshRes->GetMeshBuffer().VertexBuffer, 1);
		mCommandBuffers[imageIndex]->BindIndexBuffer(meshRes->GetMeshBuffer().IndexBuffer, GraphicsIndexType::Index32);

		mCommandBuffers[imageIndex]->BindDescriptors(&mDescriptorSet, 1);
		mCommandBuffers[imageIndex]->PushConstants({ &PushConstants, sizeof(PushConstantData) }, 0, ShaderStage::Vertex);
		mCommandBuffers[imageIndex]->DrawIndexed(indices.size(), 0, 0, 0, INSTANCE_COUNT);
		mCommandBuffers[imageIndex]->EndRendering();

		TextureBarrierUpdateDesc postRenderBarrier = {};
		postRenderBarrier.ArrayIndex = 0;
		postRenderBarrier.MipIndex = 0;
		postRenderBarrier.SourceAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
		postRenderBarrier.SourceQueue = GraphicsQueueType::Graphics;
		postRenderBarrier.SourceStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		postRenderBarrier.OldLayout = TextureMemoryLayout::ColorAttachment;

		postRenderBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::MemoryRead;
		postRenderBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		postRenderBarrier.DestinationStageFlags = PipelineStageFlags::BottomOfPipe;
		postRenderBarrier.NewLayout = TextureMemoryLayout::Present;

		postRenderBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mCommandBuffers[imageIndex]->SetTextureBarrier(mSwapchain->GetImage(imageIndex), postRenderBarrier);
		mCommandBuffers[imageIndex]->EndRecording();

		auto waitSemaphore = mSwapchain->GetImageSemaphore(imageIndex);
		auto signalSemaphore = mSwapchain->GetFlightSemaphore(imageIndex);
		auto flag = PipelineStageFlags::ColorAttachmentOutput;

		mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetDefaultPresentQueue(), &mCommandBuffers[imageIndex], 1, &waitSemaphore, 1, &flag, &signalSemaphore, 1, perFence);
		mDevice->WaitForFence(&perFence, 1);
		mDevice->ResetFences(&perFence, 1);

		mSwapchain->Present();
	}

	mDevice->WaitForIdle();
	mWindow->Hide();

#pragma endregion
	return 0;
}