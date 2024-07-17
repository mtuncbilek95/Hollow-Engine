#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class ENGINE_API DescriptorPool : public DeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, const WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), mMaxSets(desc.MaxSets),
			mPoolSizes(desc.PoolSizes)
		{}
		virtual ~DescriptorPool() override = default;

		u64 GetMaxSets() const noexcept { return mMaxSets; }
		const DArray<DescriptorPoolSize>& GetPoolSizes() const noexcept { return mPoolSizes; }

	private:
		u64 mMaxSets;
		DArray<DescriptorPoolSize> mPoolSizes;
	};
}