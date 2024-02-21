#pragma once

#include <Runtime/Graphics/Device/GraphicsDeviceObjectType.h>

namespace Hollow
{
	class GraphicsDevice;

	/**
	 * @class GraphicsDeviceObject
	 * 
	 * @brief Interface for all graphics device objects such as buffers, textures, 
	 * pipelines, etc.
	 */
	class RUNTIME_API GraphicsDeviceObject
	{
		friend class GraphicsDevice;
	public:
		GraphicsDeviceObject(GraphicsDevice* pDevice) : mOwnerDevice(pDevice) {}
		virtual ~GraphicsDeviceObject() = default;

		FORCEINLINE GraphicsDevice* GetOwnerDevice() const noexcept { return mOwnerDevice; }
		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept = 0;

		virtual void OnShutdown() noexcept = 0;

	private:
		void _SetOwnerDevice(GraphicsDevice* ownerDevice) noexcept { mOwnerDevice = ownerDevice; }

	private:
		GraphicsDevice* mOwnerDevice;
	};
}
