#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Resource/Mesh/MeshImporter.h>
#include <Runtime/Platform/PlatformMonitor.h>
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

#include <Runtime/Resource/Mesh/MeshResource.h>

#define INSTANCE_COUNT 225

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


ArrayList<Transform> InstanceTransforms(INSTANCE_COUNT);

ConstantBuffer MVPData = {
		{},
		XMMatrixLookAtLH({0, 0, -7}, {0, 0 ,0}, {0, 1, 0}),
		XMMatrixPerspectiveFovLH(XMConvertToRadians(74), static_cast<float>(1300.f / 1300.f), 0.01f, 1000.f)
};

void UpdateTransforms()
{
	for (byte i = 0; i < INSTANCE_COUNT; i++)
	{
		InstanceTransforms[i].Rotation.x += 0.1f;
		if (i % 2 == 0)
			InstanceTransforms[i].Rotation.y += 0.2f;
		else
			InstanceTransforms[i].Rotation.y -= 0.2f;

		InstanceTransforms[i].Rotation.z += 0.3f;

		MVPData.Model[i] = XMMatrixScaling(InstanceTransforms[i].Scale.x, InstanceTransforms[i].Scale.y, InstanceTransforms[i].Scale.z) *
			XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransforms[i].Rotation.y),
				XMConvertToRadians(InstanceTransforms[i].Rotation.z),
				XMConvertToRadians(InstanceTransforms[i].Rotation.x)) *
			XMMatrixTranslation(InstanceTransforms[i].Position.x, InstanceTransforms[i].Position.y, InstanceTransforms[i].Position.z);
	}
}

int main(int argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	const String& engineSourcePath = PlatformAPI::GetInstanceAPI().GetEngineSourcePath();
	String objectPath = engineSourcePath + "Resources\\Built-in\\Objects\\Cube\\Cube.gltf";
	String texturePath = engineSourcePath + "Resources\\Built-in\\Objects\\Cube\\Cube_Albedo.png";

	MeshResourceLayout meshLayout = MeshImporter::Import(objectPath);
	SharedPtr<TextureResourceLayout> texture = TextureImporter::ImportTexture(texturePath);

	// make them start at the right up corner and go left
	int sideLength = int(sqrt(INSTANCE_COUNT));
	int numCols = sideLength;

	float initialX = 1 - numCols;
	float initialY = -numCols;

	// Initialize the transforms
	for (byte i = 0; i < int(sqrt(INSTANCE_COUNT)); i++)
	{
		initialY += 1;

		for (int j = 0; j < int(sqrt(INSTANCE_COUNT)); j++)
		{
			float xPos = initialX + j;

			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position = { xPos + j, initialY + i, (float)sideLength - 1 };
			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation = { 0, 0, 0 };
			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale = { 1, 1, 1 };

			MVPData.Model[i * int(sqrt(INSTANCE_COUNT)) + j] = XMMatrixScaling(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.x, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.y, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.z) *
				XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.y),
					XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.z),
					XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.x)) *
				XMMatrixTranslation(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.x, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.y, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.z);
		}
	}

	// Get the primary monitor
	auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1300, 1300 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = WindowMode::Windowed;
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

	// Create a swapchain
	SwapchainDesc swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.ImageSize = mWindow->GetWindowResolution();
	swapchainDesc.SwapchainImageFormat = TextureFormat::RGBA8_UNorm;
	swapchainDesc.SwapchainUsage = TextureUsage::ColorAttachment;
	swapchainDesc.VSync = PresentMode::FullVSync;
	swapchainDesc.SwapchainMode = ShareMode::Exclusive;
	swapchainDesc.pQueue = GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue();

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	// ----------------- CREATE BUNDLE MEMORY -----------------

	// Create bundle memory for pre-allocation
	GraphicsMemoryDesc hostMemoryDesc = {};
	hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
	hostMemoryDesc.SizeInBytes = MB_TO_BYTE(512);

	auto mHostMemory = mDevice->CreateGraphicsMemory(hostMemoryDesc);

	GraphicsMemoryDesc deviceMemoryDesc = {};
	deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
	deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(512);

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
	depthTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1 };
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
	mDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

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

	// ----------------- CREATE MESH -----------------

	SharedPtr<MeshResource> mMesh = std::make_shared<MeshResource>();

	mMesh->ConnectMemory(mHostMemory, mDeviceMemory, true);

	for (byte i = 0; i < meshLayout.SubMeshes.size(); i++)
	{
		auto& subMesh = meshLayout.SubMeshes[i];

		uint64 perVertex = MeshImporter::CalculateTotalSubMesh(subMesh);
		uint64 perIndex  = sizeof(subMesh.Indices[i]);

		// Create mesh buffers
		mMesh->CreateMeshBuffers(perVertex, subMesh.Positions.size(), perIndex, subMesh.Indices.size());

		// Update vertex buffer
		MemoryBuffer vertexBuffer = {};

		mMesh->UpdateVertexBuffer(i, MemoryBuffer(subMesh.Positions.data(), perVertex), 0);
	}

	return 0;
}
