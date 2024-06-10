#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>

#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Memory/MemoryBuffer.h>
#include <Runtime/Graphics/Fence/Fence.h>

namespace Hollow
{
	class RUNTIME_API MeshResource final : public ResourceSubObject
	{
		struct RUNTIME_API MeshResourceData
		{
			SharedPtr<GraphicsBuffer> VertexBuffer;
			SharedPtr<GraphicsBuffer> IndexBuffer;

			SharedPtr<GraphicsBuffer> VertexStageBuffer;
			SharedPtr<GraphicsBuffer> IndexStageBuffer;
		};

	public:
		MeshResource();
		virtual ~MeshResource() override = default;

		SharedPtr<CommandBuffer> GetCommandBuffer() const { return mCommandBuffer; }
		SharedPtr<CommandPool> GetCommandPool() const { return mCommandPool; }

		SharedPtr<GraphicsMemory> GetHostMemory() const { return mHostMemory; }
		SharedPtr<GraphicsMemory> GetDeviceMemory() const { return mDeviceMemory; }

		const ArrayList<MeshResourceData>& GetMeshBuffers() const { return mMeshBuffers; }

		u64 GetTotalVertexCount() const { return mTotalVertexCount; }
		u64 GetTotalIndexCount() const { return mTotalIndexCount; }

		bool IsPreAllocated() const { return mPreAllocate; }

		SharedPtr<Fence> GetFence() const { return mFence; }

		void ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate = true);
		void CreateMeshBuffers(u32 perVertexSize, u32 resourceCount, u32 perIndexSize, u32 indexCount);
		void UpdateVertexBuffer(u32 meshIndex, MemoryOwnedBuffer& pBuffer, u32 offset);
		void UpdateIndexBuffer(u32 meshIndex, MemoryOwnedBuffer& pBuffer, u32 offset);

		virtual void OnShutdown() noexcept override;

		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Mesh; }

	private:
		void CreateInternalResources();

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;

		SharedPtr<CommandBuffer> mCommandBuffer;
		SharedPtr<CommandPool> mCommandPool;

		SharedPtr<GraphicsMemory> mHostMemory;
		SharedPtr<GraphicsMemory> mDeviceMemory;

		ArrayList<MeshResourceData> mMeshBuffers;
		u64 mTotalVertexCount;
		u64 mTotalIndexCount;

		bool mPreAllocate;

		SharedPtr<Fence> mFence;
	};
}
