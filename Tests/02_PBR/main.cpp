#include "main.h"

using namespace Hollow;

i32 main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI().InitializeArguments(argC, argV);

#pragma region Window Creation
	// Create a window
	Hollow::WindowDesc desc = {};
	desc.WindowSize = { 1300, 1300 };
	desc.WindowPosition = { 0, 0 };
	desc.WindowTitle = "Hollow - PBR";
	desc.WindowMode = WindowMode::Windowed;

	auto mWindow = WindowManager::GetAPI().InitializeWindow(desc);
#pragma endregion

#pragma region Graphics Creation
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
	swapchainDesc.pQueue = GraphicsManager::GetAPI().GetPresentQueue();

	auto mSwapchain = mDevice->CreateSwapchain(swapchainDesc);

	GraphicsManager::GetAPI().SetMemoryBudget(500, 500);
	GraphicsManager::GetAPI().SetMsaaSamples(8);
#pragma endregion

#pragma region Shader Creation
	MemoryBuffer vBuffer = {};
	MemoryOwnedBuffer vShaderCode;
	String errorMessage;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/02_PBR/Shaders/PBRVertex.vert", vBuffer);
	ShaderDesc vertexShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(vBuffer, "main", ShaderStage::Vertex, ShaderLanguage::GLSL, vShaderCode, errorMessage))
	{
		vertexShaderDesc.ShaderName = "PBRVertex";
		vertexShaderDesc.Stage = ShaderStage::Vertex;
		vertexShaderDesc.EntryPoint = "main";
		vertexShaderDesc.Language = ShaderLanguage::GLSL;
		vertexShaderDesc.ShaderCode = vShaderCode;
	}

	auto mVertexShader = mDevice->CreateShader(vertexShaderDesc);

	MemoryBuffer fBuffer = {};
	MemoryOwnedBuffer fShaderCode;
	PlatformFile::Read(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/02_PBR/Shaders/PBRFrag.frag", fBuffer);
	ShaderDesc fragmentShaderDesc = {};

	if (ShaderCompiler::CompileShaderToSPIRV(fBuffer, "main", ShaderStage::Fragment, ShaderLanguage::GLSL, fShaderCode, errorMessage))
	{
		fragmentShaderDesc.ShaderName = "PBRFragment";
		fragmentShaderDesc.Stage = ShaderStage::Fragment;
		fragmentShaderDesc.EntryPoint = "main";
		fragmentShaderDesc.Language = ShaderLanguage::GLSL;
		fragmentShaderDesc.ShaderCode = fShaderCode;
	}

	auto mFragmentShader = mDevice->CreateShader(fragmentShaderDesc);
#pragma endregion

#pragma region Fence Creation
	DArray<SharedPtr<Fence>> mRuntimeFences;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
		mRuntimeFences.push_back(mDevice->CreateSyncFence({ false }));

	auto mCompileFence = mDevice->CreateSyncFence({ false });
#pragma endregion

#pragma region Command Creation
	CommandPoolDesc commandPoolDesc = {};
	commandPoolDesc.PoolType = CommandPoolType::Graphics;

	auto mCommandPool = mDevice->CreateCommandPool(commandPoolDesc);

	CommandBufferDesc commandBufferDesc = {};
	commandBufferDesc.pOwnerPool = mCommandPool;

	auto mCompileCmd = mDevice->CreateCommandBuffer(commandBufferDesc);

	DArray<SharedPtr<CommandBuffer>> mRuntimeCmd;
	for (byte i = 0; i < swapchainDesc.BufferCount; i++)
		mRuntimeCmd.push_back(mDevice->CreateCommandBuffer(commandBufferDesc));
#pragma endregion

#pragma region RenderTarget Creation
	RenderTargetDesc presentRTVDesc = {};
	presentRTVDesc.ColorFormat = mSwapchain->GetSwapchainFormat();
	presentRTVDesc.TargetType = TextureType::Texture2D;
	presentRTVDesc.SampleCount = TextureSampleCount::Sample1;
	presentRTVDesc.ColorUsage = TextureUsage::ColorAttachment;
	presentRTVDesc.ImageSize = mSwapchain->GetImageSize();
	presentRTVDesc.ColorAttachmentCount = 0;
	presentRTVDesc.pColorTextures = mSwapchain->GetImages();
	presentRTVDesc.pColorTextureBuffers = mSwapchain->GetImageViews();
	presentRTVDesc.HasDepthTexture = false;

	// This RTV is to present the final image to the screen
	auto mPresentRenderTarget = MakeShared<RenderTarget>(presentRTVDesc);

	for (i32 i = 0; i < swapchainDesc.BufferCount; i++)
	{
		RenderTargetBarrier presentColorBarrier = {};
		presentColorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentRead;
		presentColorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		presentColorBarrier.NewLayout = TextureMemoryLayout::Present;
		presentColorBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		presentColorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mPresentRenderTarget->NewLayoutForColor(presentColorBarrier, i);
	}

	RenderTargetDesc baseRTVDesc = {};
	baseRTVDesc.ColorFormat = mSwapchain->GetSwapchainFormat();
	baseRTVDesc.DepthFormat = TextureFormat::D32_Float;
	baseRTVDesc.TargetType = GraphicsManager::GetAPI().GetMsaaSamples() > 1 ? TextureType::Texture2DMS : TextureType::Texture2D;
	baseRTVDesc.SampleCount = static_cast<TextureSampleCount>(GraphicsManager::GetAPI().GetMsaaSamples());
	baseRTVDesc.ColorUsage = TextureUsage::ColorAttachment;
	baseRTVDesc.DepthUsage = TextureUsage::DepthStencilAttachment;
	baseRTVDesc.ImageSize = mSwapchain->GetImageSize();
	baseRTVDesc.ColorAttachmentCount = 1;
	baseRTVDesc.HasDepthTexture = true;

	auto mBaseRenderTarget = MakeShared<RenderTarget>(baseRTVDesc);

	RenderTargetBarrier baseColorBarrier = {};
	baseColorBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
	baseColorBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	baseColorBarrier.NewLayout = TextureMemoryLayout::ColorAttachment;
	baseColorBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
	baseColorBarrier.AspectMask = TextureAspectFlags::ColorAspect;

	RenderTargetBarrier baseDepthBarrier = {};
	baseDepthBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::DepthStencilAttachmentWrite;
	baseDepthBarrier.NewLayout = TextureMemoryLayout::DepthStencilAttachment;
	baseDepthBarrier.DestinationQueue = GraphicsQueueType::Graphics;
	baseDepthBarrier.DestinationStageFlags = PipelineStageFlags::EarlyFragmentTests;
	baseDepthBarrier.AspectMask = TextureAspectFlags::DepthAspect;

	mBaseRenderTarget->NewLayoutForColor(baseColorBarrier, 0);
	mBaseRenderTarget->NewLayoutForDepth(baseDepthBarrier);
#pragma endregion

#pragma region Descriptor Creation
	DescriptorLayoutDesc baseVertexDSLDesc = {};
	baseVertexDSLDesc.Entries = {
		{0, DescriptorType::UniformBuffer, ShaderStage::Vertex}
	};
	auto mBaseVertexDSL = mDevice->CreateDescriptorLayout(baseVertexDSLDesc);

	DescriptorLayoutDesc baseFragmentDSLDesc = {};
	baseFragmentDSLDesc.Entries = {
		{0, DescriptorType::CombinedImageSampler, ShaderStage::Fragment},
		{1, DescriptorType::CombinedImageSampler, ShaderStage::Fragment},
		{2, DescriptorType::CombinedImageSampler, ShaderStage::Fragment},
		{3, DescriptorType::CombinedImageSampler, ShaderStage::Fragment},
		{4, DescriptorType::CombinedImageSampler, ShaderStage::Fragment}
	};
	auto mBaseFragmentDSL = mDevice->CreateDescriptorLayout(baseFragmentDSLDesc);

	DescriptorPoolDesc baseDescriptorPoolDesc = {};
	baseDescriptorPoolDesc.MaxSets = 1000;
	baseDescriptorPoolDesc.PoolSizes = {
		{DescriptorType::UniformBuffer, 1000},
		{DescriptorType::Sampler, 1000},
		{DescriptorType::SampledImage, 1000},
		{DescriptorType::StorageImage, 1000},
		{DescriptorType::StorageBuffer, 1000},
		{DescriptorType::UniformTexelBuffer, 1000},
		{DescriptorType::StorageTexelBuffer, 1000},
		{DescriptorType::InputAttachment, 1000}
	};
	auto mBaseDescriptorPool = mDevice->CreateDescriptorPool(baseDescriptorPoolDesc);

	DescriptorSetDesc baseVertexDSDesc = {};
	baseVertexDSDesc.pLayout = mBaseVertexDSL;
	baseVertexDSDesc.pOwnerPool = mBaseDescriptorPool;
	auto mBaseVertexDS = mDevice->CreateDescriptorSet(baseVertexDSDesc);

	DescriptorSetDesc baseFragmentDSDesc = {};
	baseFragmentDSDesc.pLayout = mBaseFragmentDSL;
	baseFragmentDSDesc.pOwnerPool = mBaseDescriptorPool;
	auto mBaseFragmentDS = mDevice->CreateDescriptorSet(baseFragmentDSDesc);
#pragma endregion

#pragma region Pipeline Creation
	// BlendStateAttachment	
	BlendStateAttachment blendAttachment = {};
	blendAttachment.bEnabled = true;
	blendAttachment.ColorOperation = BlendOperation::Add;
	blendAttachment.AlphaOperation = BlendOperation::Add;
	blendAttachment.SourceColorFactor = BlendFactor::SrcAlpha;
	blendAttachment.DestinationColorFactor = BlendFactor::OneMinusSrcAlpha;
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
	inputElement2.Semantic = InputElementSemantic::Normal;

	// Tangent InputElement
	InputElement inputElement3 = {};
	inputElement3.Format = TextureFormat::RGB32_Float;
	inputElement3.Semantic = InputElementSemantic::Tangent;

	// BiTangent InputElement
	InputElement inputElement4 = {};
	inputElement4.Format = TextureFormat::RGB32_Float;
	inputElement4.Semantic = InputElementSemantic::BiTangent;

	// Color InputElement
	InputElement inputElement5 = {};
	inputElement5.Format = TextureFormat::RGBA32_Float;
	inputElement5.Semantic = InputElementSemantic::Color;

	// UV InputElement
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
	multisample.bSampleShadingEnabled = GraphicsManager::GetAPI().GetMsaaSamples() > 1 ? true : false;
	multisample.Samples = static_cast<TextureSampleCount>(GraphicsManager::GetAPI().GetMsaaSamples());

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

	// Push Constant
	PushConstantRange pushConstantRange = {};
	pushConstantRange.Stage = ShaderStage::Vertex;
	pushConstantRange.Size = sizeof(PushConstant);
	pushConstantRange.Offset = 0;

	PushConstantDesc pushConstant = {};
	pushConstant.PushConstantRanges = { pushConstantRange };

	// Create Pipeline
	GraphicsPipelineDesc pipelineDesc = {};
	pipelineDesc.BlendState = blendState;
	pipelineDesc.DepthStencilState = depthStencilState;
	pipelineDesc.InputLayout = inputLayout;
	pipelineDesc.Multisample = multisample;
	pipelineDesc.RasterizerState = rasterizerState;
	pipelineDesc.ResourceLayout.ResourceLayouts = { mBaseVertexDSL, mBaseFragmentDSL };
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

#pragma region Sampler Creation
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

#pragma region Mesh Creation
	auto mMeshLayout = ResourceImporter::ImportMesh(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/DamagedHelmet.gltf");

	SharedPtr<MeshResource> mMeshResource = MakeShared<MeshResource>();
	mMeshResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);
	mMeshResource->CreateMeshBuffers(sizeof(VertexData), mMeshLayout.SubMeshes[0].Vertices.size(), sizeof(u32), mMeshLayout.SubMeshes[0].Indices.size());

	MemoryOwnedBuffer vertexBuffer = MemoryOwnedBuffer(mMeshLayout.SubMeshes[0].Vertices.data(), mMeshLayout.SubMeshes[0].Vertices.size() * sizeof(VertexData));
	MemoryOwnedBuffer indexBuffer = MemoryOwnedBuffer(mMeshLayout.SubMeshes[0].Indices.data(), mMeshLayout.SubMeshes[0].Indices.size() * sizeof(u32));
	mMeshResource->UpdateVertexBuffer(vertexBuffer, 0);
	mMeshResource->UpdateIndexBuffer(indexBuffer, 0);

	// Delete the mesh layout as it is no longer needed
	mMeshLayout.SubMeshes.clear();
	mMeshLayout.SubMeshes.shrink_to_fit();

	SharedPtr<TextureResource> mBaseTextureResource = MakeShared<TextureResource>();
	SharedPtr<TextureResource> mNormalTextureResource = MakeShared<TextureResource>();
	SharedPtr<TextureResource> mMetallicTextureResource = MakeShared<TextureResource>();
	SharedPtr<TextureResource> mAOTextureResource = MakeShared<TextureResource>();
	SharedPtr<TextureResource> mEmissiveTextureResource = MakeShared<TextureResource>();

	mBaseTextureResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);
	mNormalTextureResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);
	mMetallicTextureResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);
	mAOTextureResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);
	mEmissiveTextureResource->ConnectMemory(GraphicsManager::GetAPI().GetHostMemory(), GraphicsManager::GetAPI().GetDeviceMemory(), true);

	if (true)
	{
		auto mAlbedo = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/Textures/Default_albedo.jpg");
		auto mNormal = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/Textures/Default_normal.jpg");
		auto mMetallic = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/Textures/Default_metalRoughness.jpg");
		auto mAO = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/Textures/Default_AO.jpg");
		auto mEmissive = ResourceImporter::ImportTexture(PlatformAPI::GetAPI().GetEngineSourcePath() + "Resources/Objects/DamagedHelmet/Textures/Default_emissive.jpg");

		TextureDesc textureDesc = {};
		textureDesc.ArraySize = 1;
		textureDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
		textureDesc.ImageSize = { static_cast<u32>(mAlbedo.ImageSize.x), static_cast<u32>(mAlbedo.ImageSize.y), 1 };
		textureDesc.MipLevels = 1;
		textureDesc.SampleCount = TextureSampleCount::Sample1;
		textureDesc.Type = TextureType::Texture2D;
		textureDesc.Usage = TextureUsage::TransferDestination | TextureUsage::Sampled;
		textureDesc.pMemory = GraphicsManager::GetAPI().GetDeviceMemory();
		mBaseTextureResource->CreateTextureAndBuffer(textureDesc);
		mBaseTextureResource->UpdateTextureAndBuffer(mAlbedo.ImageData, 0);

		textureDesc.ImageSize = { static_cast<u32>(mNormal.ImageSize.x), static_cast<u32>(mNormal.ImageSize.y), 1 };
		mNormalTextureResource->CreateTextureAndBuffer(textureDesc);
		mNormalTextureResource->UpdateTextureAndBuffer(mNormal.ImageData, 0);

		textureDesc.ImageSize = { static_cast<u32>(mMetallic.ImageSize.x), static_cast<u32>(mMetallic.ImageSize.y), 1 };
		mMetallicTextureResource->CreateTextureAndBuffer(textureDesc);
		mMetallicTextureResource->UpdateTextureAndBuffer(mMetallic.ImageData, 0);

		textureDesc.ImageSize = { static_cast<u32>(mAO.ImageSize.x), static_cast<u32>(mAO.ImageSize.y), 1 };
		mAOTextureResource->CreateTextureAndBuffer(textureDesc);
		mAOTextureResource->UpdateTextureAndBuffer(mAO.ImageData, 0);

		textureDesc.ImageSize = { static_cast<u32>(mEmissive.ImageSize.x), static_cast<u32>(mEmissive.ImageSize.y), 1 };
		mEmissiveTextureResource->CreateTextureAndBuffer(textureDesc);
		mEmissiveTextureResource->UpdateTextureAndBuffer(mEmissive.ImageData, 0);
	}
#pragma endregion

#pragma region Uniform Creation
	GraphicsBufferDesc uniformBufferDesc = {};
	uniformBufferDesc.SubResourceCount = 1;
	uniformBufferDesc.SubSizeInBytes = sizeof(ModelUniform);
	uniformBufferDesc.Usage = GraphicsBufferUsage::Uniform | GraphicsBufferUsage::TransferDestination;
	uniformBufferDesc.ShareMode = ShareMode::Exclusive;
	uniformBufferDesc.pMemory = GraphicsManager::GetAPI().GetDeviceMemory();

	auto mUniformBuffer = mDevice->CreateGraphicsBuffer(uniformBufferDesc);

	GraphicsBufferDesc stagingUniformBufferDesc = {};
	stagingUniformBufferDesc.SubResourceCount = 1;
	stagingUniformBufferDesc.SubSizeInBytes = sizeof(ModelUniform);
	stagingUniformBufferDesc.Usage = GraphicsBufferUsage::TransferSource;
	stagingUniformBufferDesc.ShareMode = ShareMode::Exclusive;
	stagingUniformBufferDesc.pMemory = GraphicsManager::GetAPI().GetHostMemory();

	auto mStagingUniformBuffer = mDevice->CreateGraphicsBuffer(stagingUniformBufferDesc);
#pragma endregion

#pragma region Update Moving Data
	u64 uniformBufferSize = sizeof(ModelUniform);

	BufferDataUpdateDesc uniformDataUpdateDesc = {};
	uniformDataUpdateDesc.Memory = MemoryBuffer(&BaseModelBuffer, uniformBufferSize);
	uniformDataUpdateDesc.OffsetInBytes = 0;
	mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

	mCompileCmd->BeginRecording();

	BufferBufferCopyDesc uniformCopyDesc = {};
	uniformCopyDesc.DestinationOffset = 0;
	uniformCopyDesc.Size = uniformBufferSize;
	uniformCopyDesc.SourceOffset = 0;
	mCompileCmd->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

	DescriptorSetUpdateDesc vertexDSUpdate = {};
	vertexDSUpdate.Entries.push_back({ mUniformBuffer, nullptr, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
	mDevice->UpdateDescriptorSet(mBaseVertexDS, vertexDSUpdate);

	DescriptorSetUpdateDesc fragmentDSUpdate = {};
	fragmentDSUpdate.Entries.push_back({ mBaseTextureResource->GetTextureBuffer(), mSampler, DescriptorType::CombinedImageSampler, 1, 0, 0, 0 });
	fragmentDSUpdate.Entries.push_back({ mNormalTextureResource->GetTextureBuffer(), mSampler, DescriptorType::CombinedImageSampler, 1, 0, 0, 1 });
	fragmentDSUpdate.Entries.push_back({ mMetallicTextureResource->GetTextureBuffer(), mSampler, DescriptorType::CombinedImageSampler, 1, 0, 0, 2 });
	fragmentDSUpdate.Entries.push_back({ mAOTextureResource->GetTextureBuffer(), mSampler, DescriptorType::CombinedImageSampler, 1, 0, 0, 3 });
	fragmentDSUpdate.Entries.push_back({ mEmissiveTextureResource->GetTextureBuffer(), mSampler, DescriptorType::CombinedImageSampler, 1, 0, 0, 4 });
	mDevice->UpdateDescriptorSet(mBaseFragmentDS, fragmentDSUpdate);

	mCompileCmd->EndRecording();
	mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mCompileCmd, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);
	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);
#pragma endregion

	CORE_LOG(HE_VERBOSE, "Demanded Memory for CPU", " %.2f MB", BYTE_TO_MB(GraphicsManager::GetAPI().GetHostMemory()->GetTotalSize()));
	CORE_LOG(HE_VERBOSE, "Allocated Memory for CPU", " %.2f MB", BYTE_TO_MB(GraphicsManager::GetAPI().GetHostMemory()->GetUsedSize()));
	CORE_LOG(HE_VERBOSE, "Demanded Memory for GPU", " %.2f MB", BYTE_TO_MB(GraphicsManager::GetAPI().GetDeviceMemory()->GetTotalSize()));
	CORE_LOG(HE_VERBOSE, "Allocated Memory for GPU", " %.2f MB", BYTE_TO_MB(GraphicsManager::GetAPI().GetDeviceMemory()->GetUsedSize()));

#pragma region Main Loop
	mWindow->Show();
	while (!mWindow->IsClosed())
	{
		mWindow->PollEvents();
		UpdateBuffers();

		mSwapchain->AcquireNextImage(mCompileFence);
		mDevice->WaitForFence(&mCompileFence, 1);
		mDevice->ResetFences(&mCompileFence, 1);

		u32 imageIndex = mSwapchain->GetCurrentFrameIndex();
		auto perFence = mRuntimeFences[imageIndex];

		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = MemoryBuffer(&BaseModelBuffer, sizeof(ModelUniform));
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		mRuntimeCmd[imageIndex]->BeginRecording();

		BufferBufferCopyDesc uniformCopyDesc = {};
		uniformCopyDesc.DestinationOffset = 0;
		uniformCopyDesc.Size = uniformBufferSize;
		uniformCopyDesc.SourceOffset = 0;
		mRuntimeCmd[imageIndex]->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

		RenderTargetBarrier presentPreBarrier = {};
		presentPreBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::ColorAttachmentWrite;
		presentPreBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		presentPreBarrier.DestinationStageFlags = PipelineStageFlags::ColorAttachmentOutput;
		presentPreBarrier.NewLayout = TextureMemoryLayout::ColorAttachment;
		presentPreBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mPresentRenderTarget->NewLayoutForColor(presentPreBarrier, imageIndex);

		DynamicPassAttachmentDesc passColorAttachmentDesc = {};
		passColorAttachmentDesc.ImageBuffer = mBaseRenderTarget->GetColorBuffer(0);
		passColorAttachmentDesc.ResolveLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.ResolveBuffer = mPresentRenderTarget->GetColorBuffer(imageIndex);
		passColorAttachmentDesc.ResolveMode = ResolveModeFlags::Average;
		passColorAttachmentDesc.TextureLayout = TextureMemoryLayout::ColorAttachment;
		passColorAttachmentDesc.LoadOperation = AttachmentLoadOperation::Clear;
		passColorAttachmentDesc.StoreOperation = AttachmentStoreOperation::Store;
		passColorAttachmentDesc.ClearColor = { 0.1f, 0.2f, 0.3f, 1.0f };

		DynamicPassAttachmentDesc passDepthAttachmentDesc = {};
		passDepthAttachmentDesc.ImageBuffer = mBaseRenderTarget->GetDepthBuffer();
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

		mRuntimeCmd[imageIndex]->BeginRendering(passDesc);

		mRuntimeCmd[imageIndex]->BindPipeline(mPipeline);
		mRuntimeCmd[imageIndex]->SetViewports(&viewport, 1);
		mRuntimeCmd[imageIndex]->SetScissors(&scissor, 1);

		mRuntimeCmd[imageIndex]->BindVertexBuffers(&mMeshResource->GetMeshBuffer().VertexBuffer, 1);
		mRuntimeCmd[imageIndex]->BindIndexBuffer(mMeshResource->GetMeshBuffer().IndexBuffer, GraphicsIndexType::Index32);

		DescriptorSetUpdateDesc vertexDSUpdate = {};
		vertexDSUpdate.Entries.push_back({ mUniformBuffer, nullptr, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
		mDevice->UpdateDescriptorSet(mBaseVertexDS, vertexDSUpdate);

		mRuntimeCmd[imageIndex]->BindDescriptors(&mBaseVertexDS, 1, 0);
		mRuntimeCmd[imageIndex]->BindDescriptors(&mBaseFragmentDS, 1, 1);
		mRuntimeCmd[imageIndex]->PushConstants({ &BasePushConstant, sizeof(PushConstant) }, 0, ShaderStage::Vertex);

		mRuntimeCmd[imageIndex]->DrawIndexed(mMeshResource->GetIndexLength(), 0, 0, 0, 1);
		mRuntimeCmd[imageIndex]->EndRendering();

		mRuntimeCmd[imageIndex]->EndRecording();

		auto flag = PipelineStageFlags::ColorAttachmentOutput;
		mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetPresentQueue(), &mRuntimeCmd[imageIndex], 1, nullptr, 0, &flag, nullptr, 0, perFence);
		mDevice->WaitForFence(&perFence, 1);
		mDevice->ResetFences(&perFence, 1);

		RenderTargetBarrier presentPostBarrier = {};
		presentPostBarrier.DestinationAccessMask = GraphicsMemoryAccessFlags::MemoryRead;
		presentPostBarrier.DestinationQueue = GraphicsQueueType::Graphics;
		presentPostBarrier.DestinationStageFlags = PipelineStageFlags::BottomOfPipe;
		presentPostBarrier.NewLayout = TextureMemoryLayout::Present;
		presentPostBarrier.AspectMask = TextureAspectFlags::ColorAspect;

		mPresentRenderTarget->NewLayoutForColor(presentPostBarrier, imageIndex);

		mSwapchain->Present();
	}
#pragma endregion

	mDevice->WaitForIdle();
	mWindow->Hide();
}
