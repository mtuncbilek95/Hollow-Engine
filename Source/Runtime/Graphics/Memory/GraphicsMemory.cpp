#include "GraphicsMemory.h"

namespace Hollow
{
	GraphicsMemory::GraphicsMemory(const GraphicsMemoryDesc& desc, const SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mTotalSize(desc.SizeInBytes),
		mUsedSize(0), mMemoryType(desc.MemoryType)
	{
		SubMemory subMemory = { false, desc.SizeInBytes };
		mSubMemoryBlocks.push_back(subMemory);
	}

	u64 GraphicsMemory::AllocateSubMemory(u64 sizeInBytes)
	{
		u64 sizeLeft = mTotalSize - mUsedSize;

		if(sizeLeft < sizeInBytes)
		{
			return uint64_max;
		}

		u64 offset = 0;
		for (u32 i = 0; i < mSubMemoryBlocks.size(); ++i)
		{
			// Skip the block if it is already owned
			if (mSubMemoryBlocks[i].bOwned == true)
			{
				offset += mSubMemoryBlocks[i].SizeInBytes;
				continue;
			}

			// If the memory block is too big, skip it
			if (mSubMemoryBlocks[i].SizeInBytes < sizeInBytes)
			{
				offset += mSubMemoryBlocks[i].SizeInBytes;
				continue;
			}

			// If the memory block is just right, use it
			SubMemory newBlock = { true, sizeInBytes };
			mUsedSize += sizeInBytes;
			mSubMemoryBlocks.insert(mSubMemoryBlocks.begin() + i, newBlock);

			return offset;
		}

		return uint64_max;
	}

	void GraphicsMemory::FreeSubMemory(u64 offset)
	{
		return;
	}
}
