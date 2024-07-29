#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutDesc.h>

namespace Hollow
{
	class DescriptorLayout : public DeviceObject
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, const WeakPtr<GraphicsDevice> device) : DeviceObject(device), mEntries(desc.Entries) {}
		virtual ~DescriptorLayout() override = default;

		const DArray<DescriptorLayoutEntry>& GetEntries() const noexcept { return mEntries; }

	private:
		DArray<DescriptorLayoutEntry> mEntries;
	};
}