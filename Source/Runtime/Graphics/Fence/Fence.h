#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>

namespace Hollow
{
	class RUNTIME_API Fence : public GraphicsDeviceObject
	{
	public:
		Fence(GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) {}
		virtual ~Fence() override = default;

	private:
	};
}