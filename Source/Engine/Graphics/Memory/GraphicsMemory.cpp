#include "GraphicsMemory.h"

namespace Hollow
{
	GraphicsMemory::GraphicsMemory(const GraphicsMemoryDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
		mMemoryType(desc.MemoryType), mTotalSize(desc.MemorySize), mUsedSize(0)
	{
		SubMemory subMemory = { false, desc.MemorySize };
		mSubMemories.push_back(subMemory);
	}

	u64 GraphicsMemory::Allocate(u64 size)
	{
		u64 sizeLeft = mTotalSize - mUsedSize;

		if (sizeLeft < size)
			return u64_max;

		u64 offset = 0;
        for (auto it = mSubMemories.begin(); it != mSubMemories.end(); ++it) {
            // Skip the block if it is already owned
            if (it->bOwned) {
                offset += it->SizeInBytes;
                continue;
            }

            // If the memory block is too big, skip it
            if (it->SizeInBytes < size) {
                offset += it->SizeInBytes;
                continue;
            }

            // If the memory block is just right, use it
            SubMemory newBlock = { true, size };
            mUsedSize += size;
            mSubMemories.insert(it, newBlock);

            return offset;
        }

        return u64_max;
	}

    void GraphicsMemory::Free(u64 offset)
    {
        u32 currentOffset = 0;

        for (auto it = mSubMemories.begin(); it != mSubMemories.end(); ++it) {
            if (currentOffset == offset && it->bOwned) {
                it->bOwned = false;
                mUsedSize -= it->SizeInBytes;
                return;
            }
            currentOffset += it->SizeInBytes;
        }
    }
}
