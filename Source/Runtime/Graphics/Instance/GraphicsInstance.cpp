#include "GraphicsInstance.h"

#include <Runtime/D3D11/Instance/D3D11Instance.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsInstance::Create(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::D3D11:
			return std::make_shared<D3D11Instance>(desc);
			break;
		case GraphicsAPI::Vulkan:
			return nullptr;
		default:
			DEV_LOG(HE_ERROR, "Invalid Graphics API");
			return nullptr;
		}
	}
}
