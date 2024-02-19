#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	class RUNTIME_API DescriptorLayout : public GraphicsDeviceObject
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc);
		virtual ~DescriptorLayout() override = default;

	private:

	};
}
