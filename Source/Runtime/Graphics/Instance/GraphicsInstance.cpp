#include "GraphicsInstance.h"

#include <Runtime/Graphics/API/GraphicsManager.h>

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsInstance::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::DirectX12:
			return SharedPtr<GraphicsInstance>();
		case GraphicsAPI::Vulkan:
			return std::make_shared<VulkanInstance>(desc);
		default:
			return SharedPtr<GraphicsInstance>();
		}
	}

	GraphicsInstance::GraphicsInstance(const GraphicsInstanceDesc& desc) : Object(), mInstanceName(desc.InstanceName),
		mApplicationName(desc.ApplicationName), mGraphicsAPI(desc.API), mInstanceExtensions(desc.EnabledExtensions)
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

		GraphicsManager::GetInstanceAPI().SetGraphicsDevice(mOwnedDevice);
		return mOwnedDevice;
	}
}
