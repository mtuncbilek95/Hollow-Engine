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

using namespace Hollow;

int main(int argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);
	auto mScene = MeshImporter::Import(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/06_Sponza/Resource/Sponza.gltf");

	auto mMaterials = MeshImporter::ImportMaterial(PlatformAPI::GetInstanceAPI().GetEngineSourcePath() + "Tests/06_Sponza/Resource/Sponza.gltf", mScene);

	printf("Material Count: %zu\n", mMaterials.SubMeshMaterials.size());
}
