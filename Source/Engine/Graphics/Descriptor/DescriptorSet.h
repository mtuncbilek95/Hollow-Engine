#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Hollow
{
	class DescriptorSet : public DeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, const WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice)
		{}
		virtual ~DescriptorSet() override = default;

	private:
		WeakPtr<DescriptorPool> mOwnerPool;
		WeakPtr<DescriptorLayout> mLayout;
	};
}