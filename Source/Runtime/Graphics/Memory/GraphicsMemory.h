#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
		struct SubMemory
		{
			bool bOwned;
			uint64 SizeInBytes;
		};

	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, GraphicsDevice* pDevice);
		virtual ~GraphicsMemory() override = default;

		uint64 AllocateSubMemory(uint64 sizeInBytes);
		void FreeSubMemory(uint64 offset);

		uint64 GetSize() const { return mSizeInBytes; }
		uint64 GetUsedSizeInBytes() const { return mUsedSizeInBytes; }

		// Inherited via GraphicsDeviceObject
		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::GraphicsMemory; }

		virtual void OnShutdown() noexcept override = 0;

		private:
			GraphicsMemoryType mMemoryType;
			uint64 mSizeInBytes;
			uint64 mUsedSizeInBytes;

			arrayList<SubMemory> mSubMemories;
	};
}