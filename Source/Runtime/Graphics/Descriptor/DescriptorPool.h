#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice), mMaxSets(desc.MaxSets),
			mPoolSizes(desc.PoolSizes)
		{}
		virtual ~DescriptorPool() override = default;

		u64 GetMaxSets() const noexcept { return mMaxSets; }
		const DArray<DescriptorPoolSize>& GetPoolSizes() const noexcept { return mPoolSizes; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorPool; }

	private:
		u64 mMaxSets;
		DArray<DescriptorPoolSize> mPoolSizes;
	};
}