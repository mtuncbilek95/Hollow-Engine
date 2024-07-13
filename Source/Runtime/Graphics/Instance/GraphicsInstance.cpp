#include "GraphicsInstance.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Hollow
{
	GraphicsInstance::GraphicsInstance(const GraphicsInstanceDesc& desc) : IObject(), mInstanceName(desc.InstanceName),
		mApplicationName(desc.ApplicationName), mGraphicsAPI(desc.API)
	{
	}

	void GraphicsInstance::EnumerateAdapters()
	{
		EnumerateAdaptersImpl();
	}

	SharedPtr<GraphicsDevice> GraphicsInstance::CreateGraphicsDevice(const GraphicsDeviceDesc& desc)
	{
		switch (mGraphicsAPI)
		{
		case GraphicsAPI::DirectX12:
			return SharedPtr<GraphicsDevice>();
			break;
		case GraphicsAPI::Vulkan:
			mOwnedDevice = std::make_shared<VulkanDevice>(desc);
			break;
		default:
			return SharedPtr<GraphicsDevice>();
			break;
		}

		return mOwnedDevice;
	}
}
