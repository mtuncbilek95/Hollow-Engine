#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Hollow
{
	class ENGINE_API DescriptorSet : public DeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice)
		{}
		virtual ~DescriptorSet() override = default;

	private:
		SharedPtr<DescriptorPool> mOwnerPool;
		SharedPtr<DescriptorLayout> mLayout;
	};
}