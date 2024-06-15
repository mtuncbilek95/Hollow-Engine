#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>

#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Fence/Fence.h>

#include <Runtime/Resource/Mesh/MeshResource.h>
#include <Runtime/Resource/Material/MaterialResource.h>


namespace Hollow
{
	class RUNTIME_API SceneMesh : public ResourceSubObject
	{ 
	public:
		SceneMesh() = default;
		virtual ~SceneMesh() override = default;

		virtual void OnShutdown() noexcept override;

		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Mesh; }

	private:
		void CreateInternalResources();

	private:
		ArrayList<MeshResource> mMeshResources;
		ArrayList<MaterialResource> mMaterialResources;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		bool mPreAllocate;
		SharedPtr<Fence> mFence;

		SharedPtr<GraphicsDevice> mGraphicsDevice;

		SharedPtr<GraphicsBuffer> mUniformBuffer;
		SharedPtr<GraphicsBuffer> mUniformBufferStaging;

		SharedPtr<DescriptorSet> mDescriptorSet;
	};
}
