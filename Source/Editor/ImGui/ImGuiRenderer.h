#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Resource/Mesh/MeshResource.h>
#include <Runtime/Resource/Shader/ShaderResource.h>
#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <imgui.h>

namespace Hollow
{
	class RUNTIME_API ImGuiRenderer : public Object
	{
	public:
		ImGuiRenderer();
		virtual ~ImGuiRenderer() override = default;

		void CreateImguiResources();

		virtual void OnShutdown() override;
	private:
		ImGuiContext* mContext;

		SharedPtr<GraphicsDevice> mDevice;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<GraphicsBuffer> mUniformBuffer;
		SharedPtr<GraphicsBuffer> mUniformStagingBuffer;

		SharedPtr<MeshResource> mImguiMesh;
		SharedPtr<TextureResource> mFontTexture;
		SharedPtr<Sampler> mFontSampler;
		SharedPtr<DescriptorSet> mFontDescriptorSet;
		SharedPtr<DescriptorSet> mUniformDescriptorSet;
		SharedPtr<DescriptorLayout> mFontDescriptorLayout;
		SharedPtr<DescriptorLayout> mUniformDescriptorLayout;

		SharedPtr<ShaderResource> mImguiVertexShader;
		SharedPtr<ShaderResource> mImguiFragmentShader;

		SharedPtr<Pipeline> mImguiPipeline;
		SharedPtr<Fence> mFence;
	};
}
