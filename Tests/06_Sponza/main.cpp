#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/API/GraphicsManager.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Sampler/Sampler.h>

#include <Runtime/Resource/Mesh/MeshImporter.h>

#include <Runtime/Resource/Mesh/MeshResource.h>
#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Resource/RenderTarget/RenderTargetResource.h>

#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/Shader/Shader.h>

#include <Runtime/Resource/BasicCamera/CameraManager.h>

using namespace Hollow;

struct Transform
{
	Vector3f Position;
	Vector3f Rotation;
	Vector3f Scale;
};

struct ConstantBuffer
{
	MatrixSIMD Model;
	MatrixSIMD View;
	MatrixSIMD Projection;
};

Transform InstanceTransform = {
	{0, 0, 0},
	{0, 0, 0},
	{0.5f, 0.5f, 0.5f}
};

ConstantBuffer MVPData = {
		{ XMMatrixScaling(InstanceTransform.Scale.x, InstanceTransform.Scale.y, InstanceTransform.Scale.z) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransform.Rotation.y), XMConvertToRadians(InstanceTransform.Rotation.z), XMConvertToRadians(InstanceTransform.Rotation.x)) *
			XMMatrixTranslation(InstanceTransform.Position.x, InstanceTransform.Position.y, InstanceTransform.Position.z) },
		CameraManager::GetInstanceAPI().GetCamera()->GetViewMatrix(),
		CameraManager::GetInstanceAPI().GetCamera()->GetProjectionMatrix()
};

void UpdateMVP()
{
	MVPData.Model = XMMatrixScaling(InstanceTransform.Scale.x, InstanceTransform.Scale.y, InstanceTransform.Scale.z) *
		XMMatrixRotationRollPitchYaw(XMConvertToRadians(InstanceTransform.Rotation.y), XMConvertToRadians(InstanceTransform.Rotation.z), XMConvertToRadians(InstanceTransform.Rotation.x)) *
		XMMatrixTranslation(InstanceTransform.Position.x, InstanceTransform.Position.y, InstanceTransform.Position.z);

	MVPData.View = CameraManager::GetInstanceAPI().GetCamera()->GetViewMatrix();
	MVPData.Projection = CameraManager::GetInstanceAPI().GetCamera()->GetProjectionMatrix();
}

int main(int argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	auto mMeshPath = PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/06_Sponza/Resource/Sponza.gltf";
	auto mMeshLayout = MeshImporter::Import(mMeshPath);
	auto mMaterialLayout = MeshImporter::ImportMaterial(mMeshPath, mMeshLayout);

#pragma region Window and Graphics Initialization
	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 2560, 1440 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow - Sponza";
	desc.WindowMode = WindowMode::Fullscreen;

	auto mWindow = WindowManager::GetInstanceAPI().InitializeWindow(desc);
	CameraManager::GetInstanceAPI().GetCamera()->GetHandle();

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

	// Create bundle memory for pre-allocation on cpu
	GraphicsMemoryDesc hostMemoryDesc = {};
	hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
	hostMemoryDesc.SizeInBytes = MB_TO_BYTE(2048);

	auto mHostMemory = mDevice->CreateGraphicsMemory(hostMemoryDesc);

	// Create bundle memory for pre-allocation on gpu
	GraphicsMemoryDesc deviceMemoryDesc = {};
	deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
	deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(2048);

	auto mDeviceMemory = mDevice->CreateGraphicsMemory(deviceMemoryDesc);
#pragma endregion


#pragma region Shader Compilation
	// ----------------- CREATE SHADERS -----------------

	MemoryBuffer vBuffer = {};
	SharedPtr<MemoryOwnedBuffer> vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/06_Sponza/Shaders/testVertex.vert", vBuffer);
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
	PlatformFile::Read(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/06_Sponza/Shaders/testFrag.frag", fBuffer);
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

	// Send all color buffers to present layout from top of pipe
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

	// Send all depth buffers to depth attachment layout from top of pipe
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
		{2, DescriptorType::SampledImage, ShaderStage::Fragment},
		{3, DescriptorType::SampledImage, ShaderStage::Fragment},
		{4, DescriptorType::UniformBuffer, ShaderStage::Fragment},
		{5, DescriptorType::UniformBuffer, ShaderStage::Fragment}
	};

	auto mDescriptorLayout = mDevice->CreateDescriptorLayout(descriptorLayoutDesc);

	// Descriptor Pool
	DescriptorPoolDesc descriptorPoolDesc = {};
	descriptorPoolDesc.MaxSets = 1000;
	descriptorPoolDesc.PoolSizes = {
		{DescriptorType::UniformBuffer, 100},
		{DescriptorType::Sampler, 1},
		{DescriptorType::SampledImage, 100},
		{DescriptorType::StorageImage, 100},
		{DescriptorType::StorageBuffer, 100},
		{DescriptorType::UniformTexelBuffer, 100},
		{DescriptorType::StorageTexelBuffer, 100},
		{DescriptorType::InputAttachment, 100}
	};

	auto mDescriptorPool = mDevice->CreateDescriptorPool(descriptorPoolDesc);

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

	// Normal InputElement
	InputElement inputElement2 = {};
	inputElement2.Format = TextureFormat::RGB32_Float;
	inputElement2.Semantic = InputElementSemantic::Normal;

	// Tangent InputElement
	InputElement inputElement3 = {};
	inputElement3.Format = TextureFormat::RGB32_Float;
	inputElement3.Semantic = InputElementSemantic::Tangent;

	// Binormal InputElement
	InputElement inputElement4 = {};
	inputElement4.Format = TextureFormat::RGB32_Float;
	inputElement4.Semantic = InputElementSemantic::Binormal;

	// Color InputElement
	InputElement inputElement5 = {};
	inputElement5.Format = TextureFormat::RGBA32_Float;
	inputElement5.Semantic = InputElementSemantic::Color;

	// TexCoord InputElement
	InputElement inputElement6 = {};
	inputElement6.Format = TextureFormat::RG32_Float;
	inputElement6.Semantic = InputElementSemantic::TexCoord;

	// InputBinding
	InputBinding inputBinding = {};
	inputBinding.StepRate = InputBindingStepRate::Vertex;
	inputBinding.Elements = { inputElement1, inputElement2, inputElement3, inputElement4, inputElement5, inputElement6 };

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

#pragma region Meshes
	ArrayList<SharedPtr<MeshResource>> mMeshResources;

	for (auto& meshLayout : mMeshLayout.SubMeshes)
	{
		auto mMeshResource = std::make_shared<MeshResource>();
		mMeshResource->ConnectMemory(mHostMemory, mDeviceMemory, true);

		auto mVertexSubSize = sizeof(VertexData);
		auto mIndexSubSize = sizeof(u32);
		auto mVertexCount = meshLayout.Vertices.size();
		auto mIndexCount = meshLayout.Indices.size();

		mMeshResource->CreateMeshBuffers(mVertexSubSize, mVertexCount, mIndexSubSize, mIndexCount);
		MemoryOwnedBuffer mVertexBuffer = MemoryOwnedBuffer(meshLayout.Vertices.data(), mVertexCount * mVertexSubSize);
		MemoryOwnedBuffer mIndexBuffer = MemoryOwnedBuffer(meshLayout.Indices.data(), mIndexCount * mIndexSubSize);
		mMeshResource->UpdateVertexBuffer(mVertexBuffer, 0);
		mMeshResource->UpdateIndexBuffer(mIndexBuffer, 0);

		mMeshResources.push_back(mMeshResource);
	}

	ArrayList<SharedPtr<TextureResource>> mTextureResources;

	for (auto& materialLayout : mMaterialLayout.SubMeshMaterials)
	{
		auto mTextureResource = std::make_shared<TextureResource>();
		mTextureResource->ConnectMemory(mHostMemory, mDeviceMemory, true);

		TextureDesc textureDesc = {};
		textureDesc.ArraySize = 1;
		textureDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
		textureDesc.ImageSize = { (u32)materialLayout.BaseTexture->ImageSize.x, (u32)materialLayout.BaseTexture->ImageSize.y, 1 };
		textureDesc.MipLevels = 1;
		textureDesc.SampleCount = TextureSampleCount::Sample1;
		textureDesc.Type = TextureType::Texture2D;
		textureDesc.Usage = TextureUsage::TransferDestination | TextureUsage::Sampled;
		textureDesc.pMemory = mDeviceMemory;

		mTextureResource->CreateTextureAndBuffer(textureDesc);

		MemoryBuffer mTextureBuffer = materialLayout.BaseTexture->ImageData;
		mTextureResource->UpdateTextureAndBuffer(mTextureBuffer, 0);

		mTextureResources.push_back(mTextureResource);
	}

	ArrayList<SharedPtr<DescriptorSet>> mDescriptorSets;

	// Descriptor Set
	DescriptorSetDesc descriptorSetDesc = {};
	descriptorSetDesc.pLayout = mDescriptorLayout;
	descriptorSetDesc.pOwnerPool = mDescriptorPool;

	for (i32 i = 0; i < mTextureResources.size(); i++)
		mDescriptorSets.push_back(mDevice->CreateDescriptorSet(descriptorSetDesc));

	GraphicsBufferDesc uniformBufferDesc = {};
	uniformBufferDesc.SubResourceCount = 1;
	uniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	uniformBufferDesc.Usage = GraphicsBufferUsage::Uniform | GraphicsBufferUsage::TransferDestination;
	uniformBufferDesc.ShareMode = ShareMode::Exclusive;
	uniformBufferDesc.pMemory = mDeviceMemory;

	auto mUniformBuffer = mDevice->CreateGraphicsBuffer(uniformBufferDesc);

	GraphicsBufferDesc stagingUniformBufferDesc = {};
	stagingUniformBufferDesc.SubResourceCount = 1;
	stagingUniformBufferDesc.SubSizeInBytes = sizeof(ConstantBuffer);
	stagingUniformBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingUniformBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingUniformBufferDesc.pMemory = mHostMemory;

	auto mStagingUniformBuffer = mDevice->CreateGraphicsBuffer(stagingUniformBufferDesc);

	u64 uniformBufferSize = sizeof(ConstantBuffer);

	BufferDataUpdateDesc uniformDataUpdateDesc = {};
	uniformDataUpdateDesc.Memory = MemoryBuffer((void*)&MVPData, uniformBufferSize);
	uniformDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

#pragma endregion

	mCommandBuffer->BeginRecording();

	BufferBufferCopyDesc uniformCopyDesc = {};
	uniformCopyDesc.DestinationOffset = 0;
	uniformCopyDesc.Size = uniformBufferSize;
	uniformCopyDesc.SourceOffset = 0;

	mCommandBuffer->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

	for (i32 i = 0; i < mTextureResources.size(); i++)
	{
		DescriptorSetUpdateDesc descriptorUpdateDesc = {};
		descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
		descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
		descriptorUpdateDesc.Entries.push_back({ mTextureResources[i]->GetTextureBuffer(), DescriptorType::SampledImage, 1, 0, 0, 2 });

		mDevice->UpdateDescriptorSet(mDescriptorSets[i], descriptorUpdateDesc);
	}

	mCommandBuffer->EndRecording();

	mDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);

#pragma region Main Loop
	mWindow->Show();
	while (!mWindow->IsClosed())
	{
		mSwapchain->AcquireNextImage();
		u32 imageIndex = mSwapchain->GetCurrentFrameIndex();

		mWindow->PollEvents();

		CameraManager::GetInstanceAPI().GetCamera()->UpdateInput(0.16f);
		CameraManager::GetInstanceAPI().GetCamera()->Update(0.16f);

		UpdateMVP();
		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = MemoryBuffer(&MVPData, sizeof(ConstantBuffer));
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		auto perFence = mRuntimeFences[imageIndex];

		mCommandBuffers[imageIndex]->BeginRecording();

		BufferBufferCopyDesc constantCopyDesc = {};
		constantCopyDesc.DestinationOffset = 0;
		constantCopyDesc.Size = constantDataUpdateDesc.Memory.GetSize();
		constantCopyDesc.SourceOffset = 0;
		mCommandBuffers[imageIndex]->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, constantCopyDesc);

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
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.02f, 0.03f, 1.0f };

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

		for (i32 i = 0; i < mMeshResources.size(); i++)
		{
			auto vBuffer = mMeshResources[i]->GetMeshBuffer().VertexBuffer;
			auto iBuffer = mMeshResources[i]->GetMeshBuffer().IndexBuffer;

			mCommandBuffers[imageIndex]->BindVertexBuffers(&vBuffer, 1);
			mCommandBuffers[imageIndex]->BindIndexBuffer(iBuffer, GraphicsIndexType::Index32);

			auto mDescSet = mDescriptorSets[i];
			DescriptorSetUpdateDesc descriptorUpdateDesc = {};
			descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
			descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
			descriptorUpdateDesc.Entries.push_back({ mTextureResources[i]->GetTextureBuffer(), DescriptorType::SampledImage, 1, 0, 0, 2 });

			mDevice->UpdateDescriptorSet(mDescSet, descriptorUpdateDesc);

			mCommandBuffers[imageIndex]->BindDescriptors(&mDescSet, 1);
			mCommandBuffers[imageIndex]->DrawIndexed(mMeshLayout.SubMeshes[i].Indices.size(), 0, 0, 0, 1);
		}

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
