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

		uint64 GetTotalVertexCount() const { return mTotalVertexCount; }
		uint64 GetTotalIndexCount() const { return mTotalIndexCount; }

		bool IsPreAllocated() const { return mPreAllocate; }

		SharedPtr<Fence> GetFence() const { return mFence; }

		void ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate = true);
		void CreateMeshBuffers(uint32 perVertex, uint32 vertexCount, uint32 perIndex, uint32 indexCount);
		void UpdateVertexBuffer(uint32 meshIndex, MemoryBuffer pBuffer, uint32 offset);
		void UpdateIndexBuffer(uint32 meshIndex, MemoryBuffer pBuffer, uint32 offset);

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
		uint64 mTotalVertexCount;
		uint64 mTotalIndexCount;

		bool mPreAllocate;

		SharedPtr<Fence> mFence;
	};
}
