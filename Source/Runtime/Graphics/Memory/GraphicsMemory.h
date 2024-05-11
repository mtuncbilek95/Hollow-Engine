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
			uint64 SizeInBytes;
		};

	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, const SharedPtr<GraphicsDevice> device);
		virtual ~GraphicsMemory() override = default;

		uint64 AllocateSubMemory(uint64 sizeInBytes);
		void FreeSubMemory(uint64 offset);

		uint64 GetTotalSize() const { return mTotalSize; }
		uint64 GetUsedSize() const { return mUsedSize; }
		uint64 GetFreeSize() const { return mTotalSize - mUsedSize; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::GraphicsMemory; }

		virtual void OnShutdown() override = 0;

	private:
		GraphicsMemoryType mMemoryType;
		uint64 mTotalSize;
		uint64 mUsedSize;

		ArrayList<SubMemory> mSubMemoryBlocks;
	};
}
