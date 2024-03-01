#include <Window/Window.h>
#include <Renderer/Renderer.h>
#include <Shader/Shader.h>
#include <ShaderCompiler/ShaderCompiler.h>

#include <RenderPass/RenderPass.h>
#include <Pipeline/Pipeline.h>


struct Vertex
{
	Vector2f pos;
	Vector3f color;
};


int main()
{
	// Create a window
	MiniVk::WindowDesc winDesc = {};
	winDesc.Windowed = true;
	winDesc.WindowPosition = { (2560 - 1920) / 2, (1440 - 1080) / 2 };
	winDesc.WindowSize = { 1920, 1080 };
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
	vShaderDesc.SourceString = MiniVk::ShaderCompiler::ReadFile(R"(D:\Projects\MiniVulkan\Source\Shaders\testVertex.vert)");
	vShaderDesc.Stage = MiniVk::ShaderStage::Vertex;
	MiniVk::Shader* pVertexShader = pRenderer->CreateShader(vShaderDesc);

	// Create a fragment shader
	MiniVk::ShaderDesc fShaderDesc = {};
	fShaderDesc.EntryPoint = "main";
	fShaderDesc.Language = MiniVk::ShaderLanguage::GLSL;
	fShaderDesc.ShaderName = "testFragment";
	fShaderDesc.SourceString = MiniVk::ShaderCompiler::ReadFile(R"(D:\Projects\MiniVulkan\Source\Shaders\testFrag.frag)");
	fShaderDesc.Stage = MiniVk::ShaderStage::Fragment;
	MiniVk::Shader* pFragmentShader = pRenderer->CreateShader(fShaderDesc);

	// Create a render Pass
	MiniVk::RenderPassDesc mainPassDesc = {};
	MiniVk::RenderPass* pMainPass = pRenderer->CreateRenderPass(mainPassDesc);

	// Create a pipeline
	MiniVk::GraphicsPipelineDesc pipelineDesc = {};
	pipelineDesc.pRenderPass = pMainPass;
	pipelineDesc.Shaders = { pVertexShader, pFragmentShader };
	MiniVk::Pipeline* pPipeline = pRenderer->CreatePipeline(pipelineDesc);

	uint32 imageIndex = 0;
	while (!pWindow->ShouldClose())
	{
		pWindow->PollMessages();

		pRenderer->BeginRecording(&imageIndex);
		pRenderer->BindRenderPass(pPipeline, imageIndex);
		pRenderer->Draw(imageIndex);
		pRenderer->EndRecording(imageIndex);
		pRenderer->Present(imageIndex);

		imageIndex = (imageIndex + 1) % pRenderer->GetVkSwapchainImages().size();
	}

	return 0;
}