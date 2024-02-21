#include "GraphicsMemory.h"

namespace Hollow
{
	GraphicsMemory::GraphicsMemory(const GraphicsMemoryDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mType(desc.Type), mSizeInBytes(desc.SizeInBytes),
		mUsedSizeInBytes(0)
	{
		SubAllocation subBlock = {};
		subBlock.Size = mSizeInBytes;
		subBlock.bOwnedByMemory = false;
		mSubAllocations.push_back(subBlock);
	}

	uint64 GraphicsMemory::Allocate(uint64 sizeInBytes)
	{
		// Check if there is enough space in the memory
		const uint64 leftSize = mSizeInBytes - mUsedSizeInBytes;
		if (leftSize < sizeInBytes)
		{
			return uint64_max;
		}


		// Find the first suballocation that has enough space
		uint64 offset = 0;
		for (int i = 0; i < mSubAllocations.size(); i++)
		{
			SubAllocation& block = mSubAllocations[i];
			// First check if owned, so offset can be moved.
			// If owned, move to the next suballocation
			if (block.bOwnedByMemory)
			{
				offset += block.Size;
				continue;
			}

			// Check if there is enough space in the suballocation
			// If not, move to the next suballocation
			if (block.Size < mSizeInBytes)
			{
				offset += block.Size;
				continue;
			}

			// the suballocation or main block has enough space, give info about 
			// how much space will left in the block, then allocate the space.
			const uint64 leftSize = block.Size - sizeInBytes;
			block.Size = leftSize;

			SubAllocation newBlock = {};
			newBlock.Size = sizeInBytes;
			newBlock.bOwnedByMemory = true;
			mSubAllocations.insert(mSubAllocations.begin() + i, newBlock); // if later there is enough space, the new block can be inserted in between.

			mUsedSizeInBytes += sizeInBytes; // update the used size of the memory

			return offset; // return the offset of the allocated space
		}

		return uint64_max; // Memory allocation failed
	}

	void GraphicsMemory::Free(uint64 offset)
	{
		// Iterate through the suballocations to find the one with the given offset
		for (int i = 0; i < mSubAllocations.size(); i++)
		{
			SubAllocation& block = mSubAllocations[i];

			// Check if this block matches the given offset
			if (offset == 0 && block.bOwnedByMemory)
			{
				// Mark the block as not owned by memory
				block.bOwnedByMemory = false;

				// Update the used size of the memory
				mUsedSizeInBytes -= block.Size;

				// Check if the previous block can be merged
				if (i > 0 && !mSubAllocations[i - 1].bOwnedByMemory)
				{
					mSubAllocations[i - 1].Size += block.Size;
					mSubAllocations.erase(mSubAllocations.begin() + i);
				}

				// Check if the next block can be merged
				if (i < mSubAllocations.size() - 1 && !mSubAllocations[i + 1].bOwnedByMemory)
				{
					mSubAllocations[i].Size += mSubAllocations[i + 1].Size;
					mSubAllocations.erase(mSubAllocations.begin() + i + 1);
				}

				return; // Memory freed successfully
			}

			// Move to the next block
			offset -= block.Size;
		}
	}


}