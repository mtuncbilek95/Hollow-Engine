#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace Hollow
{
	class GraphicsDevice;
	class Fence : public DeviceObject
	{
	public:
		Fence(bool bSignalled, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), mSignalled(bSignalled) {}
		~Fence() override = default;

		bool IsSignalled() const { return mSignalled; }

	private:
		bool mSignalled;
	};
}
