#include "GraphicsManager.h"

namespace Hollow
{
	void GraphicsManager::SetGraphicsDevice(SharedPtr<GraphicsDevice> device)
	{
		if (mGraphicsDevice)
		{
			CORE_LOG(HE_WARNING, "Graphics Manager", "Graphics Device already set.");
			return;
		}

		mGraphicsDevice = device;
	}
}
