#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>

#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Fence/Fence.h>

namespace Hollow
{
	class RUNTIME_API MaterialResource : public ResourceSubObject
	{
		struct RUNTIME_API MaterialResourceData
		{
			SharedPtr<Texture> BaseColorTexture;
			SharedPtr<TextureBuffer> BaseColorTextureBuffer;

			SharedPtr<GraphicsBuffer> BaseColorStageBuffer;
		};

	public:
		MaterialResource() = default;
		~MaterialResource() = default;

		virtual void OnShutdown() noexcept override;

		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Material; }

	private:
		void CreateInternalResources();

	private:
		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		bool mPreAllocate;
		SharedPtr<Fence> mFence;
	};
}
