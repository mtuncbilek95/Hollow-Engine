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

#define CO_FACTOR .5f
#define MSAA_SAMPLES 8
using namespace Hollow;

struct Transform
{
	Vec3f Position;
	Vec3f Rotation;
	Vec3f Scale;
};

struct Camera
{
	Vec3f Position;
	Vec3f Up;
	Vec3f Right;
	Vec3f Forward;
};

struct ConstantBuffer
{
	Matrix4f Model;
};

Transform InstanceTransform
{
	.Position = Vec3f(0, 0, 0),
	.Rotation = Vec3f(0, 0, 0),
	.Scale = Vec3f(1, 1, 1)
};

Camera MainCamera = {
	.Position = Vec3f(0, 2, -0.5),
	.Up = Vec3f(0, -1, 0),
	.Right = Vec3f(1, 0, 0),
	.Forward = Vec3f(0, 0, 1)
};

ConstantBuffer MVPData = {
	.Model = Mat4f(1)
};

struct SupportBuffer
{
	Mat4f View;
	Mat4f Projection;
};

SupportBuffer SupportData = {
	.View = Mat4f(1),
	.Projection = Mat4f(1)
};

void UpdateTransforms()
{

	Mat4f model = Mat4f(1.0f);
	model = Math::translate(model, InstanceTransform.Position);
	model = Math::rotate(model, InstanceTransform.Rotation.x, Vec3f(1, 0, 0));
	model = Math::rotate(model, InstanceTransform.Rotation.y, Vec3f(0, 1, 0));
	model = Math::rotate(model, InstanceTransform.Rotation.z, Vec3f(0, 0, 1));
	model = Math::scale(model, InstanceTransform.Scale);

	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_W) == GLFW_PRESS)
		MainCamera.Position += MainCamera.Forward * CO_FACTOR;
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_S) == GLFW_PRESS)
		MainCamera.Position -= MainCamera.Forward * CO_FACTOR;
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_A) == GLFW_PRESS)
		MainCamera.Position -= glm::cross({ 0,1,0 }, MainCamera.Forward) * CO_FACTOR;
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_D) == GLFW_PRESS)
		MainCamera.Position += glm::cross({ 0,1,0 }, MainCamera.Forward) * CO_FACTOR;
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
		MainCamera.Position -= MainCamera.Up * CO_FACTOR;
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		MainCamera.Position += MainCamera.Up * CO_FACTOR;

	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		Mat4f rotMatrix(1);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(-0.8f), glm::vec3(0, 1, 0));
		MainCamera.Forward = glm::vec3(rotMatrix * glm::vec4(MainCamera.Forward, 1.0));
	}
	if (glfwGetKey(WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle(), GLFW_KEY_E) == GLFW_PRESS)
	{
		Mat4f rotMatrix(1);
		rotMatrix = glm::rotate(rotMatrix, glm::radians(0.8f), glm::vec3(0, 1, 0));
		MainCamera.Forward = glm::vec3(rotMatrix * glm::vec4(MainCamera.Forward, 1.0));
	}

	SupportData.View = Math::lookAt(MainCamera.Position, MainCamera.Position + MainCamera.Forward, MainCamera.Up);
	SupportData.Projection = Math::perspective(Math::radians(70.f), (1300.f / 1300.f), 0.1f, 100.f);

	MVPData.Model = Math::transpose(SupportData.Projection * SupportData.View * model);
}

int main(int argC, char** argV)
{
	// Initialize the platform API
	PlatformAPI::GetAPI().InitializeArguments(argC, argV);

#pragma region Object Cacophony

	Mat4f rotMatrix(1);
	rotMatrix = glm::rotate(rotMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
	MainCamera.Forward = glm::vec3(rotMatrix * glm::vec4(MainCamera.Forward, 1.0));

	SupportData.View = Math::lookAt(MainCamera.Position, MainCamera.Position + MainCamera.Forward, MainCamera.Up);
	SupportData.Projection = Math::perspective(Math::radians(70.f), (1300.f / 1300.f), 0.1f, 100.f);

	Mat4f model = Mat4f(1.0f);
	model = Math::translate(model, InstanceTransform.Position);
	model = Math::rotate(model, InstanceTransform.Rotation.x, Vec3f(1, 0, 0));
	model = Math::rotate(model, InstanceTransform.Rotation.y, Vec3f(0, 1, 0));
	model = Math::rotate(model, InstanceTransform.Rotation.z, Vec3f(0, 0, 1));
	model = Math::scale(model, InstanceTransform.Scale);

	MVPData.Model = Math::transpose(SupportData.Projection * SupportData.View * model);

	auto meshLayout = ResourceImporter::ImportMesh(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/01_Runtime/Resources/Sponza/Sponza.gltf");
	auto materialLayout = ResourceImporter::ImportMaterial(PlatformAPI::GetAPI().GetEngineSourcePath() + "Tests/01_Runtime/Resources/Sponza/Sponza.gltf");

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
	hostMemoryDesc.SizeInBytes = MB_TO_BYTE(110);

	auto mHostMemory = mDevice->CreateGraphicsMemory(hostMemoryDesc);

	GraphicsMemoryDesc deviceMemoryDesc = {};
	deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
	deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(220);

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
	descriptorPoolDesc.MaxSets = 1000;
	descriptorPoolDesc.PoolSizes = {
		{DescriptorType::UniformBuffer, 1000},
		{DescriptorType::Sampler, 1000},
		{DescriptorType::SampledImage, 1000},
		{DescriptorType::StorageImage, 1000},
		{DescriptorType::StorageBuffer, 1000},
		{DescriptorType::UniformTexelBuffer, 1000},
		{DescriptorType::StorageTexelBuffer, 1000},
		{DescriptorType::InputAttachment, 1000}
	};

	auto mDescriptorPool = mDevice->CreateDescriptorPool(descriptorPoolDesc);

	DArray<SharedPtr<DescriptorSet>> mDescriptorSets;

	for (i32 i = 0; i < meshLayout.SubMeshes.size(); i++)
	{
		// Descriptor Set
		DescriptorSetDesc descriptorSetDesc = {};
		descriptorSetDesc.pLayout = mDescriptorLayout;
		descriptorSetDesc.pOwnerPool = mDescriptorPool;

		auto dSet = mDevice->CreateDescriptorSet(descriptorSetDesc);
		mDescriptorSets.push_back(dSet);
	}
#pragma endregion

#pragma region Pipeline Initialization
	// ----------------- CREATE PIPELINE -----------------

	// BlendStateAttachment	
	BlendStateAttachment blendAttachment = {};
	blendAttachment.bEnabled = false;
	blendAttachment.ColorOperation = BlendOperation::Max;
	blendAttachment.AlphaOperation = BlendOperation::Max;
	blendAttachment.SourceColorFactor = BlendFactor::One;
	blendAttachment.DestinationColorFactor = BlendFactor::Zero;
	blendAttachment.SourceAlphaFactor = BlendFactor::One;
	blendAttachment.DestinationAlphaFactor = BlendFactor::Zero;
	blendAttachment.WriteMask = BlendColorWriteMask::All;

	// Create BlendState
	BlendStateDesc blendState = {};
	blendState.bLogicOperationEnabled = false;
	blendState.LogicOperation = LogicOperation::Clear;
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
	inputElement2.Format = TextureFormat::RGBA32_Float;
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

#pragma region Buffer Initialization
	// ----------------- CREATE OBJECT BUFFER -----------------

	DArray<SharedPtr<MeshResource>> mMeshResources;
	HashMap<u32, SharedPtr<TextureResource>> mTextureResources;

	for (auto& mesh : meshLayout.SubMeshes)
	{
		auto meshRes = MakeShared<MeshResource>();

		meshRes->ConnectMemory(mHostMemory, mDeviceMemory, true);
		meshRes->CreateMeshBuffers(sizeof(VertexData), mesh.Vertices.size(), sizeof(u32), mesh.Indices.size());

		MemoryOwnedBuffer vertexBuffer = MemoryOwnedBuffer(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(VertexData));
		MemoryOwnedBuffer indexBuffer = MemoryOwnedBuffer(mesh.Indices.data(), mesh.Indices.size() * sizeof(u32));
		meshRes->UpdateVertexBuffer(vertexBuffer, 0);
		meshRes->UpdateIndexBuffer(indexBuffer, 0);

		mMeshResources.push_back(meshRes);
	}

	for (auto& material : materialLayout.SubMeshMaterials)
	{
		auto texRes = MakeShared<TextureResource>();
		texRes->ConnectMemory(mHostMemory, mDeviceMemory, true);

		TextureDesc textureDesc = {};
		textureDesc.ArraySize = 1;
		textureDesc.ImageFormat = TextureFormat::RGBA8_UNorm;
		textureDesc.ImageSize = { static_cast<u32>(material.BaseTexture.ImageSize.x), static_cast<u32>(material.BaseTexture.ImageSize.y), 1 };
		textureDesc.MipLevels = 1;
		textureDesc.SampleCount = TextureSampleCount::Sample1;
		textureDesc.Type = TextureType::Texture2D;
		textureDesc.Usage = TextureUsage::TransferDestination | TextureUsage::Sampled;
		textureDesc.pMemory = mDeviceMemory;

		texRes->CreateTextureAndBuffer(textureDesc);
		texRes->UpdateTextureAndBuffer(material.BaseTexture.ImageData, 0);

		mTextureResources[material.MaterialIndex] = texRes;
	}

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

	u64 uniformBufferSize = sizeof(ConstantBuffer);

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

	for (i32 i = 0; i < mDescriptorSets.size(); i++)
	{
		DescriptorSetUpdateDesc descriptorUpdateDesc = {};
		descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
		descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
		descriptorUpdateDesc.Entries.push_back({ mTextureResources[meshLayout.SubMeshes[i].MaterialIndex]->GetTextureBuffer(), DescriptorType::SampledImage, 1, 0, 0, 2 });

		mDevice->UpdateDescriptorSet(mDescriptorSets[i], descriptorUpdateDesc);
	}

	mCommandBuffer->EndRecording();
	mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mCompileFence);

	mDevice->WaitForFence(&mCompileFence, 1);
	mDevice->ResetFences(&mCompileFence, 1);

#pragma endregion

	CORE_LOG(HE_VERBOSE, "Allocated Memory for CPU", " %.2f MB", BYTE_TO_MB(mHostMemory->GetUsedSize()));
	CORE_LOG(HE_VERBOSE, "Allocated Memory for GPU", " %.2f MB", BYTE_TO_MB(mDeviceMemory->GetUsedSize()));

#pragma region Main Loop
	mWindow->Show();
	while (!mWindow->IsClosed())
	{
		mWindow->PollEvents();
		UpdateTransforms();
		
		mSwapchain->AcquireNextImage(mCompileFence);
		mDevice->WaitForFence(&mCompileFence, 1);
		mDevice->ResetFences(&mCompileFence, 1);

		u32 imageIndex = mSwapchain->GetCurrentFrameIndex();
		auto perFence = mRuntimeFences[imageIndex];

		BufferDataUpdateDesc constantDataUpdateDesc;
		constantDataUpdateDesc.Memory = MemoryBuffer(&MVPData, sizeof(ConstantBuffer));
		constantDataUpdateDesc.OffsetInBytes = 0;
		mDevice->UpdateBufferData(mStagingUniformBuffer, uniformDataUpdateDesc);

		mCommandBuffers[imageIndex]->BeginRecording();

		BufferBufferCopyDesc uniformCopyDesc = {};
		uniformCopyDesc.DestinationOffset = 0;
		uniformCopyDesc.Size = uniformBufferSize;
		uniformCopyDesc.SourceOffset = 0;
		mCommandBuffers[imageIndex]->CopyBufferToBuffer(mStagingUniformBuffer, mUniformBuffer, uniformCopyDesc);

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
		//mCommandBuffers[imageIndex]->SetViewports(&viewport, 1);
		//mCommandBuffers[imageIndex]->SetScissors(&scissor, 1);

		i32 index = 0;
		for (auto& meshRes : mMeshResources)
		{
			mCommandBuffers[imageIndex]->BindVertexBuffers(&meshRes->GetMeshBuffer().VertexBuffer, 1);
			mCommandBuffers[imageIndex]->BindIndexBuffer(meshRes->GetMeshBuffer().IndexBuffer, GraphicsIndexType::Index32);

			auto descriptorSet = mDescriptorSets[index];

			DescriptorSetUpdateDesc descriptorUpdateDesc = {};
			descriptorUpdateDesc.Entries.push_back({ mUniformBuffer, DescriptorType::UniformBuffer, 1, 0, 0, 0 });
			descriptorUpdateDesc.Entries.push_back({ mSampler, DescriptorType::Sampler, 1, 0, 0, 1 });
			descriptorUpdateDesc.Entries.push_back({ mTextureResources[meshLayout.SubMeshes[index].MaterialIndex]->GetTextureBuffer(), DescriptorType::SampledImage, 1, 0, 0, 2 });

			mDevice->UpdateDescriptorSet(descriptorSet, descriptorUpdateDesc);

			mCommandBuffers[imageIndex]->BindDescriptors(&descriptorSet, 1);
			mCommandBuffers[imageIndex]->DrawIndexed(meshLayout.SubMeshes[index].Indices.size(), 0, 0, 0, 1);
			index++;
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

		mDevice->SubmitToQueue(GraphicsManager::GetAPI().GetDefaultPresentQueue(), &mCommandBuffers[imageIndex], 1, nullptr, 0, &flag, nullptr, 0, perFence);
		mDevice->WaitForFence(&perFence, 1);
		mDevice->ResetFences(&perFence, 1);

		mSwapchain->Present();
	}

	mDevice->WaitForIdle();
	mWindow->Hide();

#pragma endregion
	return 0;
}