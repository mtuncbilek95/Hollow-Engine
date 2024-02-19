#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>

namespace Hollow
{
	class RUNTIME_API CommandPool : public GraphicsDeviceObject
	{
	public:
		CommandPool(const CommandPoolDesc& desc);
		virtual ~CommandPool() override = default;

	private:
		
	};
}
