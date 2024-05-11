#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice), mType(desc.Type), 
			mPoolSizes(desc.PoolSizes)
		{}
		virtual ~DescriptorPool() override = default;

		GraphicsMemoryType GetMemoryType() const noexcept { return mType; }
		const ArrayList<DescriptorPoolSize>& GetPoolSizes() const noexcept { return mPoolSizes; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorPool; }
		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsMemoryType mType;
		ArrayList<DescriptorPoolSize> mPoolSizes;
	};
}