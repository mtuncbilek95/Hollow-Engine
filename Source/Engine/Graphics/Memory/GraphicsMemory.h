#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class GraphicsMemory : public DeviceObject
	{
		struct SubMemory
		{
			bool bOwned;
			u64 SizeInBytes;
		};
	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, WeakPtr<GraphicsDevice> pDevice);
		virtual ~GraphicsMemory() override = default;

		u64 Allocate(u64 size);
		void Free(u64 offset);

		GraphicsMemoryType GetMemoryType() const { return mMemoryType; }
		u64 GetTotalMemorySize() const { return mTotalSize; }
		u64 GetUsedMemorySize() const { return mUsedSize; }
		u64 GetFreeMemorySize() const { return mTotalSize - mUsedSize; }

	private:
		GraphicsMemoryType mMemoryType;
		u64 mTotalSize;
		u64 mUsedSize;

		List<SubMemory> mSubMemories;
	};
}
