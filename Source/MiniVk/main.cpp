#include <Window/Window.h>
#include <Renderer/Renderer.h>
#include <Shader/Shader.h>
#include <ShaderCompiler/ShaderCompiler.h>

#include <RenderPass/RenderPass.h>
#include <Pipeline/Pipeline.h>
#include <Buffer/Buffer.h>
#include <Descriptor/DescriptorSet.h>
#include <Descriptor/DescriptorPool.h>
#include <Descriptor/DescriptorLayout.h>


struct ConstantBufferObject
{
	Matrix4f model;
	Matrix4f view;
	Matrix4f proj;
};

struct Vertex
{
	Vector2f pos;
	Vector3f color;
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
	MiniVk::WindowDesc winDesc = {};
	winDesc.Windowed = true;
	winDesc.WindowPosition = { (1920 - 1280) / 2, (1080 - 720) / 2 };
	winDesc.WindowSize = { 1280, 720 };
	winDesc.WindowTitle = "MiniVk";
	MiniVk::Window* pWindow = new MiniVk::Window(winDesc);

	// Create a Renderer
	MiniVk::RendererDesc rendDesc = {};
	rendDesc.WindowHandle = pWindow->GetWindowHandle();
	rendDesc.WindowNativeHandle = pWindow->GetWindowNativeHandle();
	rendDesc.WindowSize = pWindow->GetWindowSize();
	MiniVk::Renderer* pRenderer = new MiniVk::Renderer(rendDesc);
	pRenderer->Initialize();

	// Create a vertex shader
	MiniVk::ShaderDesc vShaderDesc = {};
	vShaderDesc.EntryPoint = "main";
	vShaderDesc.Language = MiniVk::ShaderLanguage::GLSL;
	vShaderDesc.ShaderName = "testVertex";
	vShaderDesc.SourceString = MiniVk::ShaderCompiler::ReadFile(R"(D:\Projects\Hollow-Engine\Source\Shaders\testVertex.vert)");
	vShaderDesc.Stage = MiniVk::ShaderStage::Vertex;
	MiniVk::Shader* pVertexShader = pRenderer->CreateShader(vShaderDesc);

	// Create a fragment shader
	MiniVk::ShaderDesc fShaderDesc = {};
	fShaderDesc.EntryPoint = "main";
	fShaderDesc.Language = MiniVk::ShaderLanguage::GLSL;
	fShaderDesc.ShaderName = "testFragment";
	fShaderDesc.SourceString = MiniVk::ShaderCompiler::ReadFile(R"(D:\Projects\Hollow-Engine\Source\Shaders\testFrag.frag)");
	fShaderDesc.Stage = MiniVk::ShaderStage::Fragment;
	MiniVk::Shader* pFragmentShader = pRenderer->CreateShader(fShaderDesc);

	// Create a render Pass
	MiniVk::RenderPassDesc mainPassDesc = {};
	MiniVk::RenderPass* pMainPass = pRenderer->CreateRenderPass(mainPassDesc);

	// Create a descriptor layout
	MiniVk::DescriptorLayoutDesc layoutDesc = {};
	layoutDesc.LayoutEntries = {
		{MiniVk::DescriptorType::UniformBuffer, MiniVk::ShaderStage::Vertex, 0},
		{MiniVk::DescriptorType::UniformBuffer, MiniVk::ShaderStage::Vertex, 1},
		{MiniVk::DescriptorType::UniformBuffer, MiniVk::ShaderStage::Vertex, 2}
	};

	MiniVk::DescriptorLayout* pDescriptorLayout = pRenderer->CreateDescriptorLayout(layoutDesc);

	// Create a descriptor pool
	MiniVk::DescriptorPoolDesc poolDesc = {};
	poolDesc.MaxSets = 2;
	poolDesc.DescriptorEntries = {
		{MiniVk::DescriptorType::UniformBuffer, 3},
		{MiniVk::DescriptorType::UniformBuffer, 3}
	};

	MiniVk::DescriptorPool* pDescriptorPool = pRenderer->CreateDescriptorPool(poolDesc);

	// Create a descriptor set 1
	MiniVk::DescriptorSetDesc setDesc = {};
	setDesc.pLayout = pDescriptorLayout;
	setDesc.pPool = pDescriptorPool;

	MiniVk::DescriptorSet* pDescriptorSet1 = pRenderer->CreateDescriptorSet(setDesc);

	// Create a descriptor set 2
	MiniVk::DescriptorSetDesc setDesc2 = {};
	setDesc2.pLayout = pDescriptorLayout;
	setDesc2.pPool = pDescriptorPool;

	MiniVk::DescriptorSet* pDescriptorSet2 = pRenderer->CreateDescriptorSet(setDesc2);

	// Create a pipeline
	MiniVk::GraphicsPipelineDesc pipelineDesc = {};
	pipelineDesc.pRenderPass = pMainPass;
	pipelineDesc.Shaders = { pVertexShader, pFragmentShader };
	pipelineDesc.InputStride = sizeof(Vertex);
	pipelineDesc.InputOffsets[0] = 0;
	pipelineDesc.InputOffsets[1] = 8;
	pipelineDesc.DescriptorLayouts = {pDescriptorLayout};

	MiniVk::Pipeline* pPipeline = pRenderer->CreatePipeline(pipelineDesc);

	// Create a vertex staging buffer
	MiniVk::BufferDesc stagingBufferDesc = {};
	stagingBufferDesc.SizeInBytes = sizeof(vertices[0]) * vertices.size();
	stagingBufferDesc.Usage = MiniVk::BufferUsage::TransferSrc;

	MiniVk::Buffer* pVertexStagingBuffer = pRenderer->CreateBuffer(stagingBufferDesc);

	// Bind the vertex staging buffer
	pRenderer->BindBuffer(pVertexStagingBuffer, (void*)vertices.data());

	// Create a vertex buffer
	MiniVk::BufferDesc vertexBufferDesc = {};
	vertexBufferDesc.SizeInBytes = sizeof(vertices[0]) * vertices.size();
	vertexBufferDesc.Usage = MiniVk::BufferUsage::TransferDst | MiniVk::BufferUsage::Vertex;

	MiniVk::Buffer* pVertexBuffer = pRenderer->CreateBuffer(vertexBufferDesc);

	pRenderer->CopyBuffer(pVertexStagingBuffer, pVertexBuffer, vertexBufferDesc.SizeInBytes);

	delete pVertexStagingBuffer;

	// Create an index staging buffer
	MiniVk::BufferDesc indexStagingBufferDesc = {};
	indexStagingBufferDesc.SizeInBytes = sizeof(uint32) * indices.size();
	indexStagingBufferDesc.Usage = MiniVk::BufferUsage::TransferSrc;

	MiniVk::Buffer* pIndexStagingBuffer = pRenderer->CreateBuffer(indexStagingBufferDesc);

	// Bind the index staging buffer
	pRenderer->BindBuffer(pIndexStagingBuffer, (void*)indices.data());

	// Create an index buffer
	MiniVk::BufferDesc indexBufferDesc = {};
	indexBufferDesc.SizeInBytes = sizeof(uint32) * indices.size();
	indexBufferDesc.Usage = MiniVk::BufferUsage::TransferDst | MiniVk::BufferUsage::Index;

	MiniVk::Buffer* pIndexBuffer = pRenderer->CreateBuffer(indexBufferDesc);

	pRenderer->CopyBuffer(pIndexStagingBuffer, pIndexBuffer, indexBufferDesc.SizeInBytes);

	delete pIndexStagingBuffer;

	// Create a uniform staging buffer
	MiniVk::BufferDesc uniformStagingBufferDesc = {};
	uniformStagingBufferDesc.SizeInBytes = sizeof(ConstantBufferObject);
	uniformStagingBufferDesc.Usage = MiniVk::BufferUsage::TransferSrc;

	MiniVk::Buffer* pUniformStagingBuffer = pRenderer->CreateBuffer(uniformStagingBufferDesc);

	// Bind the uniform staging buffer
	ConstantBufferObject ubo = {};
	ubo.model = XMMatrixTranspose(XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixRotationZ(0.0f) * XMMatrixScaling(1.0f, 1.0f, 1.0f));
	ubo.view = XMMatrixTranspose(XMMatrixLookAtLH({ 0.0f, 0.0f, -2.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
	ubo.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), winDesc.WindowSize.x / winDesc.WindowSize.y, 0.1f, 100.0f));

	pRenderer->BindBuffer(pUniformStagingBuffer, &ubo);

	// Create a uniform buffer
	MiniVk::BufferDesc uniformBufferDesc = {};
	uniformBufferDesc.SizeInBytes = sizeof(ConstantBufferObject);
	uniformBufferDesc.Usage = MiniVk::BufferUsage::Uniform | MiniVk::BufferUsage::TransferDst;

	MiniVk::Buffer* pUniformBuffer = pRenderer->CreateBuffer(uniformBufferDesc);

	pRenderer->CopyBuffer(pUniformStagingBuffer, pUniformBuffer, uniformBufferDesc.SizeInBytes);

	delete pUniformStagingBuffer;

	uint32 imageIndex = 0;
	while (!pWindow->ShouldClose())
	{
		pWindow->PollMessages();

		pRenderer->BeginRecording(&imageIndex);
		pRenderer->BindRenderPass(pPipeline, imageIndex);
		pRenderer->DrawIndexed(pVertexBuffer, pIndexBuffer, vertices.size(), indices.size(), imageIndex);
		pRenderer->EndRecording(imageIndex);
		pRenderer->Present(imageIndex);

		imageIndex = (imageIndex + 1) % pRenderer->GetVkSwapchainImages().size();
	}

	return 0;
}