#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorPool : public GraphicsDeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc);
		virtual ~DescriptorPool() override = default;

	private:

	};
}
