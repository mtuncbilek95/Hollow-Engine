#include "GraphicsAdapter.h"

#include <Runtime/Graphics/Manager/GraphicsManager.h>

namespace Hollow
{
	GraphicsAdapter::GraphicsAdapter(const GraphicsAdapterDesc& desc) : mProductName(desc.ProductName), mVRam(desc.VRam), mInstance(desc.pInstance)
	{
	}
	void GraphicsAdapter::CreateDevice(const GraphicsDeviceDesc& desc)
	{
		mDevice = CreateDeviceCore(desc);
	}
}
