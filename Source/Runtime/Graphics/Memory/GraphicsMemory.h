#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsMemory : public GraphicsDeviceObject
	{
	public:
		GraphicsMemory(const GraphicsMemoryDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), 
			mMemoryType(desc.MemoryType), mOwnerBuffer(desc.pOwnerBuffer)
		{}
		virtual ~GraphicsMemory() override = default;

		// Inherited via GraphicsDeviceObject
		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::GraphicsMemory; }

		virtual void OnShutdown() noexcept override = 0;

		private:
			GraphicsBuffer* mOwnerBuffer;
			GraphicsMemoryType mMemoryType;
	};
}