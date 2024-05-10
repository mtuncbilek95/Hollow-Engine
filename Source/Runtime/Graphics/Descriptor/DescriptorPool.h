#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mType(desc.Type), mPoolSizes(desc.PoolSizes)
		{}
		virtual ~DescriptorPool() override = default;

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorPool; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsMemoryType mType;
		arrayList<DescriptorPoolSize> mPoolSizes;
	};
}