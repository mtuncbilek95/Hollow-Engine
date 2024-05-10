#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	class RUNTIME_API GraphicsManager final : public ManagerAPI<GraphicsManager>
	{
		friend class GraphicsInstance;

	public:
		GraphicsManager() = default;
		~GraphicsManager() override = default;

		SharedPtr<GraphicsDevice> GetDefaultDevice() noexcept { return mGraphicsDevice; }

	private:
		void SetGraphicsDevice(SharedPtr<GraphicsDevice> device);

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
	};
}
