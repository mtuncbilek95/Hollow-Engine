#include "D3D11Adapter.h"

#include <Runtime/D3D11/Device/D3D11Device.h>

namespace Hollow
{
	D3D11Adapter::D3D11Adapter(const GraphicsAdapterDesc& desc, ComPtr<IDXGIAdapter1> adapter) : GraphicsAdapter(desc)
	{
		mD3DAdapter = adapter;
	}

	SharedPtr<GraphicsDevice> D3D11Adapter::CreateDeviceCore(const GraphicsDeviceDesc& desc)
	{
		DEV_LOG(HE_INFO, "Used %s to create D3D11 Device", GetProductName().c_str());

		return std::make_shared<D3D11Device>(desc);
	}
}
