#include "MeshResource.h"

#include <Runtime/Graphics/API/GraphicsManager.h>

namespace Hollow
{
	MeshResource::MeshResource() : ResourceSubObject(), mPreAllocate(false), mTotalVertexCount(0), mTotalIndexCount(0)
	{
		mGraphicsDevice = GraphicsManager::GetInstanceAPI().GetDefaultDevice();
		CreateInternalResources();
	}

	void MeshResource::ConnectMemory(const SharedPtr<GraphicsMemory>& hostMemory, const SharedPtr<GraphicsMemory>& deviceMemory, bool bPreAllocate)
	{
		mHostMemory = hostMemory;
		mDeviceMemory = deviceMemory;
		mPreAllocate = bPreAllocate;
	}

	void MeshResource::CreateMeshBuffers(u32 perVertex, u32 vertexCount, u32 perIndex, u32 indexCount)
	{
		MeshResourceData meshData = {};

		// Create vertex buffer
		GraphicsBufferDesc vertexDesc = {};
		vertexDesc.pMemory = mDeviceMemory;
		vertexDesc.ShareMode = ShareMode::Exclusive;
		vertexDesc.SubResourceCount = vertexCount;
		vertexDesc.SubSizeInBytes = perVertex;
		vertexDesc.Usage = GraphicsBufferUsage::Vertex | GraphicsBufferUsage::TransferDestination;
		
		meshData.VertexBuffer = mGraphicsDevice->CreateGraphicsBuffer(vertexDesc);

		// If the memory is pre-allocated, create a staging buffer
		if (mPreAllocate)
		{
			GraphicsBufferDesc stageDesc = {};
			stageDesc.pMemory = mHostMemory;
			stageDesc.ShareMode = ShareMode::Exclusive;
			stageDesc.SubResourceCount = vertexCount;
			stageDesc.SubSizeInBytes = perVertex;
			stageDesc.Usage = GraphicsBufferUsage::TransferSource;

			meshData.VertexStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(stageDesc);
		}

		// Create index buffer
		GraphicsBufferDesc indexDesc = {};
		indexDesc.pMemory = mDeviceMemory;
		indexDesc.ShareMode = ShareMode::Exclusive;
		indexDesc.SubResourceCount = indexCount;
		indexDesc.SubSizeInBytes = perIndex;
		indexDesc.Usage = GraphicsBufferUsage::Index | GraphicsBufferUsage::TransferDestination;

		meshData.IndexBuffer = mGraphicsDevice->CreateGraphicsBuffer(indexDesc);

		// If the memory is pre-allocated, create a staging buffer
		if (mPreAllocate)
		{
			GraphicsBufferDesc stageDesc = {};
			stageDesc.pMemory = mHostMemory;
			stageDesc.ShareMode = ShareMode::Exclusive;
			stageDesc.SubResourceCount = indexCount;
			stageDesc.SubSizeInBytes = perIndex;
			stageDesc.Usage = GraphicsBufferUsage::TransferSource;

			meshData.IndexStageBuffer = mGraphicsDevice->CreateGraphicsBuffer(stageDesc);
		}

		mMeshBuffer = meshData;
		mTotalIndexCount = indexCount;
		mTotalVertexCount = vertexCount;
	}

	void MeshResource::UpdateVertexBuffer(MemoryOwnedBuffer& pBuffer, u32 offset)
	{
		// If the memory is not pre-allocated, create a staging buffer
		if(!mPreAllocate)
		{
			GraphicsBufferDesc desc = {};
			desc.pMemory = mHostMemory;
			desc.ShareMode = ShareMode::Exclusive;
			desc.SubResourceCount = mMeshBuffer.VertexBuffer->GetSubResourceCount();
			desc.SubSizeInBytes = mMeshBuffer.VertexBuffer->GetSubSizeInBytes();
			desc.Usage = GraphicsBufferUsage::TransferSource;

			SharedPtr<GraphicsBuffer> stagingBuffer = mGraphicsDevice->CreateGraphicsBuffer(desc);
		}

		// Update the staging buffer
		BufferDataUpdateDesc updateDesc = {};
		updateDesc.Memory = pBuffer;
		updateDesc.OffsetInBytes = offset;
		mGraphicsDevice->UpdateBufferData(mMeshBuffer.VertexStageBuffer, updateDesc);

		// Copy the data from the staging buffer to the vertex buffer
		mCommandBuffer->BeginRecording();

		BufferBufferCopyDesc copyDesc = {};
		copyDesc.DestinationOffset = offset;
		copyDesc.SourceOffset = 0;
		copyDesc.Size = pBuffer.GetSize();
		mCommandBuffer->CopyBufferToBuffer(mMeshBuffer.VertexStageBuffer, mMeshBuffer.VertexBuffer, copyDesc);

		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);
	}

	void MeshResource::UpdateIndexBuffer(MemoryOwnedBuffer& pBuffer, u32 offset)
	{
		if (!mPreAllocate)
		{
			GraphicsBufferDesc desc = {};
			desc.pMemory = mHostMemory;
			desc.ShareMode = ShareMode::Exclusive;
			desc.SubResourceCount = mMeshBuffer.IndexBuffer->GetSubResourceCount();
			desc.SubSizeInBytes = mMeshBuffer.IndexBuffer->GetSubSizeInBytes();
			desc.Usage = GraphicsBufferUsage::TransferSource;

			SharedPtr<GraphicsBuffer> stagingBuffer = mGraphicsDevice->CreateGraphicsBuffer(desc);
		}

		BufferDataUpdateDesc updateDesc = {};
		updateDesc.Memory = pBuffer;
		updateDesc.OffsetInBytes = offset;
		mGraphicsDevice->UpdateBufferData(mMeshBuffer.IndexStageBuffer, updateDesc);

		// Copy the data from the staging buffer to the index buffer
		mCommandBuffer->BeginRecording();

		BufferBufferCopyDesc copyDesc = {};
		copyDesc.DestinationOffset = offset;
		copyDesc.SourceOffset = 0;
		copyDesc.Size = pBuffer.GetSize();
		mCommandBuffer->CopyBufferToBuffer(mMeshBuffer.IndexStageBuffer, mMeshBuffer.IndexBuffer, copyDesc);

		mCommandBuffer->EndRecording();

		mGraphicsDevice->SubmitToQueue(GraphicsManager::GetInstanceAPI().GetDefaultPresentQueue(), &mCommandBuffer, 1, nullptr, 0, nullptr, nullptr, 0, mFence);

		mGraphicsDevice->WaitForFence(&mFence, 1);
		mGraphicsDevice->ResetFences(&mFence, 1);
	}

	void MeshResource::OnShutdown() noexcept
	{
	}

	void MeshResource::CreateInternalResources()
	{
		CommandPoolDesc poolDesc = { CommandPoolType::Graphics };
		mCommandPool = mGraphicsDevice->CreateCommandPool(poolDesc);

		CommandBufferDesc bufferDesc = { mCommandPool };
		mCommandBuffer = mGraphicsDevice->CreateCommandBuffer(bufferDesc);

		FenceDesc fenceDesc = { false };
		mFence = mGraphicsDevice->CreateSyncFence(fenceDesc);
	}
}
