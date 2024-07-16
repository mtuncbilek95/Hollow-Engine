#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace Hollow
{
	class ENGINE_API GraphicsDevice;
	class ENGINE_API Fence : public DeviceObject
	{
	public:
		Fence(bool bSignalled, SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), mSignalled(bSignalled) {}
		~Fence() override = default;

		bool IsSignalled() const { return mSignalled; }

	private:
		bool mSignalled;
	};
}
