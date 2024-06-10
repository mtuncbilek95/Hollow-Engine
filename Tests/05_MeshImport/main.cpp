#include <Runtime/Platform/PlatformAPI.h>
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
#include <Runtime/Resource/RenderTarget/RenderTargetResource.h>
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
#include <Runtime/Resource/Mesh/MeshImporter.h>

#define INSTANCE_COUNT 49

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
		XMMatrixPerspectiveFovLH(XMConvertToRadians(74), static_cast<float>(2560.f / 1440.f), 0.01f, 1000.f)
};

void UpdateTransforms()
{
	for (byte i = 0; i < INSTANCE_COUNT; i++)
	{
		if (i % 2 == 0)
		{
			InstanceTransforms[i].Rotation.x += 0.009f;
			InstanceTransforms[i].Rotation.y += 0.023f;
			InstanceTransforms[i].Rotation.z += 0.026f;

		}
		else
		{
			InstanceTransforms[i].Rotation.x -= 0.019f;
			InstanceTransforms[i].Rotation.y -= 0.014f;
			InstanceTransforms[i].Rotation.z -= 0.037f;
		}


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

#pragma region Object Cacophony
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
			float xPos = initialX + j * 1.2;

			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position = { xPos + j, initialY + i * 1.2f, (float)sideLength - 1 };
			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation = { 0, -90.f, 180.f };
			InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale = { 1, 1, 1 };

			MVPData.Model[i * int(sqrt(INSTANCE_COUNT)) + j] = XMMatrixScaling(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.x, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.y, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Scale.z) *
				XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.y),
					XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.z),
					XMConvertToRadians(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Rotation.x)) *
				XMMatrixTranslation(InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.x, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.y, InstanceTransforms[i * int(sqrt(INSTANCE_COUNT)) + j].Position.z);
		}
	}
#pragma endregion

#pragma region Window and Graphics Initialization
	// Get the primary monitor
	auto primaryMonitor = PlatformMonitor::GetPrimaryMonitor();

	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1920, 1080 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow Engine";
	desc.WindowMode = WindowMode::Windowed;
	desc.pMonitor = primaryMonitor;

	auto mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);

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
	swapchainDesc.pQueue = GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue();

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);
#pragma endregion

#pragma region Pre-Allocated Memory
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
#pragma endregion

#pragma region Shader Compilation
	// ----------------- CREATE SHADERS -----------------

	MemoryBuffer vBuffer = {};
	SharedPtr<MemoryOwnedBuffer> vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/05_MeshImport/Shaders/testVertex.vert", vBuffer);
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
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/05_MeshImport/Shaders/testFrag.frag", fBuffer);
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

	ArrayList<SharedPtr<Fence>> mRuntimeFences;
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

	ArrayList<SharedPtr<CommandBuffer>> mCommandBuffers;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
	{
		mCommandBuffers.push_back(mDevice->CreateCommandBuffer(commandBufferDesc));
	}
#pragma endregion

#pragma region Depth and Multisample Textures
	// ----------------- CREATE DEPTH TEXTURE -----------------

	TextureDesc colorTextureDesc = {};
	colorTextureDesc.ArraySize = 1;
	colorTextureDesc.ImageFormat = swapchainDesc.SwapchainImageFormat;
	colorTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1 };
	colorTextureDesc.MipLevels = 1;
	colorTextureDesc.SampleCount = TextureSampleCount::Sample8;
	colorTextureDesc.Type = TextureType::Texture2DMS;
	colorTextureDesc.Usage = TextureUsage::ColorAttachment;
	colorTextureDesc.pMemory = mDeviceMemory;

	TextureDesc depthTextureDesc = {};
	depthTextureDesc.ArraySize = 4;
	depthTextureDesc.ImageFormat = TextureFormat::D32_Float;
	depthTextureDesc.ImageSize = { mSwapchain->GetImageSize().x, mSwapchain->GetImageSize().y, 1 };
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.SampleCount = TextureSampleCount::Sample8;
	depthTextureDesc.Type = TextureType::Texture2DMS;
	depthTextureDesc.Usage = TextureUsage::DepthStencilAttachment;
	depthTextureDesc.pMemory = mDeviceMemory;

	RenderTargetDesc rTargetDesc = {};
	rTargetDesc.mRegionSize = mSwapchain->GetImageSize();
	rTargetDesc.DescStruct.ColorAttachmentCount = mSwapchain->GetBufferCount();
	rTargetDesc.DescStruct.ColorAttachment = colorTextureDesc;
	rTargetDesc.DescStruct.HasDepthAttachment = true;
	rTargetDesc.DescStruct.DepthAttachment = depthTextureDesc;

	auto mRenderTarget = RenderTargetResource::CreateRenderTarget(rTargetDesc);
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

	mCommandBuffer->SetTextureBarrier(mRenderTarget->GetDepthBuffer()->GetTexture(), depthTextureBarrier);

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

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

	// TexCoord InputElement
	InputElement inputElement3 = {};
	inputElement3.Format = TextureFormat::RG32_Float;
	inputElement3.Semantic = InputElementSemantic::TexCoord;

	// InputBinding
	InputBinding inputBinding = {};
	inputBinding.StepRate = InputBindingStepRate::Vertex;
	inputBinding.Elements = { inputElement1, inputElement3 };

	// Create InputLayout
	InputLayoutDesc inputLayout = {};
	inputLayout.Bindings = { inputBinding };
	inputLayout.Topology = MeshTopology::TriangleList;

	// Create MultisampleDesc
	MultisampleDesc multisample = {};
	multisample.bSampleShadingEnabled = true;
	multisample.Samples = TextureSampleCount::Sample8;

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

	auto mMeshData = MeshImporter::Import(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/05_MeshImport/Resource/DamagedHelmet.gltf");
	auto texture = TextureImporter::ImportTexture(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/05_MeshImport/Resource/Default_albedo.jpg");

	auto mSubSize = MeshImporter::CalculateTotalMesh(mMeshData);
	auto mIndexSize = MeshImporter::CalculateIndexSize(mMeshData);

	auto vertices = MeshImporter::MeshToMemoryBuffer(mMeshData);
	auto indices = MeshImporter::IndexToMemoryBuffer(mMeshData);

#pragma region Buffer Initialization
	// ----------------- CREATE OBJECT BUFFER -----------------

	GraphicsBufferDesc vertexBufferDesc = {};
	vertexBufferDesc.SubResourceCount = 1;
	vertexBufferDesc.SubSizeInBytes = mSubSize;
	vertexBufferDesc.Usage = GraphicsBufferUsage::Vertex | GraphicsBufferUsage::TransferDestination;
	vertexBufferDesc.ShareMode = ShareMode::Exclusive;
	vertexBufferDesc.pMemory = mDeviceMemory;

	auto mVertexBuffer = mDevice->CreateGraphicsBuffer(vertexBufferDesc);

	GraphicsBufferDesc indexBufferDesc = {};
	indexBufferDesc.SubResourceCount = 1;
	indexBufferDesc.SubSizeInBytes = mIndexSize;
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
	stagingVertexBufferDesc.SubResourceCount = 1;
	stagingVertexBufferDesc.SubSizeInBytes = mSubSize;
	stagingVertexBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingVertexBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingVertexBufferDesc.pMemory = mHostMemory;

	auto mStagingVertexBuffer = mDevice->CreateGraphicsBuffer(stagingVertexBufferDesc);

	GraphicsBufferDesc stagingIndexBufferDesc = {};
	stagingIndexBufferDesc.SubResourceCount = 1;
	stagingIndexBufferDesc.SubSizeInBytes = mIndexSize;
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

	uint64 uniformBufferSize = sizeof(ConstantBuffer);

	BufferDataUpdateDesc vertexDataUpdateDesc = {};
	vertexDataUpdateDesc.Memory = *vertices;
	vertexDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingVertexBuffer, vertexDataUpdateDesc);

	BufferDataUpdateDesc indexDataUpdateDesc = {};
	indexDataUpdateDesc.Memory = *indices;
	indexDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingIndexBuffer, indexDataUpdateDesc);

	BufferDataUpdateDesc uniformDataUpdateDesc = {};
	uniformDataUpdateDesc.Memory = { (void*)&MVPData, uniformBufferSize };
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
	vertexCopyDesc.Size = mSubSize;
	vertexCopyDesc.SourceOffset = 0;
	mCommandBuffer->CopyBufferToBuffer(mStagingVertexBuffer, mVertexBuffer, vertexCopyDesc);

	BufferBufferCopyDesc indexCopyDesc = {};
	indexCopyDesc.DestinationOffset = 0;
	indexCopyDesc.Size = mIndexSize;
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
	mDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);

#pragma endregion

#pragma region Main Loop
	mWindow->Show();
	while (mWindow->IsVisible())
	{
		mSwapchain->AcquireNextImage();

		uint32 imageIndex = mSwapchain->GetCurrentFrameIndex();
		auto perFence = mRuntimeFences[imageIndex];

		UpdateTransforms();

		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = { &MVPData, sizeof(ConstantBuffer) };
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		mWindow->PollEvents();

		mCommandBuffers[imageIndex]->BeginRecording();

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
		passColorAttachmentDesc.ImageBuffer = mRenderTarget->GetColorBuffer(imageIndex);
		passColorAttachmentDesc.ResolveLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.ResolveBuffer = mSwapchain->GetImageView(imageIndex);
		passColorAttachmentDesc.ResolveMode = ResolveModeFlags::Average;
		passColorAttachmentDesc.TextureLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassAttachmentDesc passDepthAttachmentDesc = {};
		passDepthAttachmentDesc.ImageBuffer = mRenderTarget->GetDepthBuffer();
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
		mCommandBuffers[imageIndex]->BindVertexBuffers(&mVertexBuffer, 1);
		mCommandBuffers[imageIndex]->BindIndexBuffer(mIndexBuffer, GraphicsIndexType::Index32);

		mCommandBuffers[imageIndex]->BindDescriptors(&mDescriptorSet, 1);
		mCommandBuffers[imageIndex]->DrawIndexed(mMeshData.Indices.size(), 0, 0, 0, INSTANCE_COUNT);
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

		mDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffers[imageIndex], 1, &waitSemaphore, 1, &flag, &signalSemaphore, 1, perFence);
		mDevice->WaitForFence(&perFence, 1);
		mDevice->ResetFences(&perFence, 1);

		mSwapchain->Present();
	}
	mWindow->Hide();

	mDevice->WaitForIdle();
	mDevice->OnShutdown();
	mGraphicsInstance->OnShutdown();
}