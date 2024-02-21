#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
		struct SubAllocation
		{
			bool bOwnedByMemory;
			uint64 Size;
		};

	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, GraphicsDevice* pDevice);
		virtual ~GraphicsMemory() override = default;

		uint64 Allocate(uint64 sizeInBytes);
		void Free(uint64 offset);

		FORCEINLINE GraphicsMemoryType GetMemoryType() const noexcept { return mType; }
		FORCEINLINE uint64 GetSizeInBytes() const noexcept { return mSizeInBytes; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::GraphicsMemory; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsMemoryType mType;
		uint64 mSizeInBytes;
		uint64 mUsedSizeInBytes;
		Array<SubAllocation> mSubAllocations;
	};
}
