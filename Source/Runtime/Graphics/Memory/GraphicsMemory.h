#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc) : mType(desc.Type), mSizeInBytes(desc.SizeInBytes) 
		{}
		virtual ~GraphicsMemory() override = default;

		FORCEINLINE GraphicsMemoryType GetMemoryType() const noexcept { return mType; }
		FORCEINLINE uint64 GetSizeInBytes() const noexcept { return mSizeInBytes; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::GraphicsMemory; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsMemoryType mType;
		uint64 mSizeInBytes;
	};
}
