#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace Hollow
{
	class GraphicsDevice;
	class Semaphore : public DeviceObject
	{
	public:
		Semaphore(WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice){}
		~Semaphore() override = default;
	};
}
