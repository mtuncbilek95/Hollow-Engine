#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Manager.h>

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Hollow
{
	class RUNTIME_API GraphicsManager : public Manager<GraphicsManager>
	{
	public:
		GraphicsManager() = default;
		~GraphicsManager() = default;
	};
}