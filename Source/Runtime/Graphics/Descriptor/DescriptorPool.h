#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mMemoryType(desc.MemoryType), 
			mPoolSizes(desc.PoolSizes), mSetCount(desc.SetCount) {}
		virtual ~DescriptorPool() override = default;
		
		DescriptorMemoryType GetMemoryType() const { return mMemoryType; }
		const Array<DescriptorPoolSize>& GetPoolSizes() const { return mPoolSizes; }
		uint64 GetSetCount() const { return mSetCount; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept { return GraphicsDeviceObjectType::DescriptorPool; }

		virtual void OnShutdown() noexcept override = 0;
	private:
		DescriptorMemoryType mMemoryType;
		Array<DescriptorPoolSize> mPoolSizes;
		uint64 mSetCount;
	};
}
