#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorLayout : public GraphicsDeviceObject
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) {}
		virtual ~DescriptorLayout() override = default;

	private:

	};
}
