#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	/**
	 * @class GraphicsMemory
	 * @brief Represents a block of memory that can be used for CPU and GPU resources.
	 */
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
		struct RUNTIME_API SubMemory
		{
			bool bOwned;
			u64 SizeInBytes;
		};

	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, const SharedPtr<GraphicsDevice> device);
		virtual ~GraphicsMemory() override = default;

		u64 AllocateSubMemory(u64 sizeInBytes);
		void FreeSubMemory(u64 offset);

		u64 GetTotalSize() const { return mTotalSize; }
		u64 GetUsedSize() const { return mUsedSize; }
		u64 GetFreeSize() const { return mTotalSize - mUsedSize; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::GraphicsMemory; }

	private:
		GraphicsMemoryType mMemoryType;
		u64 mTotalSize;
		u64 mUsedSize;

		DArray<SubMemory> mSubMemoryBlocks;
	};
}
