#include "GraphicsMemory.h"

namespace Hollow
{
	GraphicsMemory::GraphicsMemory(const GraphicsMemoryDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mMemoryType(desc.MemoryType), mSizeInBytes(desc.SizeInBytes), mUsedSizeInBytes(0)
	{
		// Allocate memory
		SubMemory firstSubMemory = {};
		firstSubMemory.bOwned = false;
		firstSubMemory.SizeInBytes = desc.SizeInBytes;
		mSubMemories.push_back(firstSubMemory);
	}

	uint64 GraphicsMemory::AllocateSubMemory(uint64 sizeInBytes)
	{
		// Check if there is enough space in the memory
		uint64 sizeLeft = mSizeInBytes - mUsedSizeInBytes;
		if (sizeLeft < sizeInBytes)
			return uint64_max;

		// Look for an available space
		uint64 offset = 0;
		for (uint32 i = 0; i < mSubMemories.size(); i++)
		{
			// Skip the submemory if it is owned
			if (mSubMemories[i].bOwned == true)
			{
				offset += mSubMemories[i].SizeInBytes;
				continue;
			}

			// If the submemory is too big, skip it
			if (mSubMemories[i].SizeInBytes < sizeInBytes)
			{
				offset += mSubMemories[i].SizeInBytes;
				continue;
			}

			uint64 sizeLeft = mSubMemories[i].SizeInBytes - sizeInBytes;
			SubMemory newSubMemory = {};
			newSubMemory.SizeInBytes = sizeInBytes;
			newSubMemory.bOwned = true;
			mUsedSizeInBytes += sizeInBytes;
			mSubMemories.insert(mSubMemories.begin() + i, newSubMemory);

			return offset;
		}

		return uint64_max;
	}
}