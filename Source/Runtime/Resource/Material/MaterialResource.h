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
		struct RUNTIME_API MaterialBufferData
		{
			SharedPtr<GraphicsBuffer> UniformMaterialBuffer;
			SharedPtr<GraphicsBuffer> UniformMaterialBufferStaging;
		};
	public:
		MaterialResource() = default;
		~MaterialResource() = default;

	private:
		void CreateInternalResources();
	private:
		MaterialBufferData mMaterialBuffers;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		bool mPreAllocate;
		SharedPtr<Fence> mFence;
	};
}
