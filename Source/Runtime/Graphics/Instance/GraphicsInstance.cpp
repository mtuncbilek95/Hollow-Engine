#include "GraphicsInstance.h"

#include <Runtime/Graphics/GraphicsManager.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsInstance::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::Vulkan:
			return SharedPtr<GraphicsInstance>();
		case GraphicsAPI::DirectX12:
			return SharedPtr<GraphicsInstance>();
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
		SharedPtr<GraphicsDevice> device = CreateDeviceImpl(desc);

		mOwnedDevice = device;
		GraphicsManager::GetInstanceAPI().SetGraphicsDevice(mOwnedDevice);

		return device;
	}
}
