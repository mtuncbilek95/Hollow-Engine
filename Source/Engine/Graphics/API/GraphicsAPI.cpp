#include "GraphicsAPI.h"

#include <Engine/Vulkan/Instance/VInstance.h>
#include <Engine/Vulkan/Device/VDevice.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsAPI::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		mInstance = MakeShared<VInstance>(desc);
		return mInstance;
	}

	SharedPtr<GraphicsDevice> GraphicsAPI::CreateDevice()
	{
		mDevice = MakeShared<VDevice>(mInstance->GetSharedPtrAs<VInstance>());
		return mDevice;
	}
}
