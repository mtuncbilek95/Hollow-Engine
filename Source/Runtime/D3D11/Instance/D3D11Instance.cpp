#include "D3D11Instance.h"

#include <Runtime/D3D11/Adapter/D3D11Adapter.h>

namespace Hollow
{
	D3D11Instance::D3D11Instance(const GraphicsInstanceDesc& desc) : GraphicsInstance(desc)
	{
		DEV_ASSERT(SUCCEEDED(CreateDXGIFactory1(__uuidof(IDXGIFactory), &mD3DInstance)), "D3D11Instance", "Failed to create DXGIFactory");

		DEV_LOG(HE_INFO, "DXGIFactory has been created successfully.");

		// Enumarate all the adapters
		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; mD3DInstance->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 adapterDesc;
			adapter->GetDesc1(&adapterDesc);

			WString wstr = adapterDesc.Description;
			int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
			String strTo(requiredSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], requiredSize, nullptr, nullptr);

			DEV_LOG(HE_INFO, "Adapter: %s has been found.", strTo.c_str());

			if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				DEV_LOG(HE_WARNING, "Software Adapter: %s has been ignored", strTo.c_str());
				continue;
			}

			GraphicsAdapterDesc d3dDesc;
			d3dDesc.ProductName = strTo;
			d3dDesc.VRam = adapterDesc.DedicatedVideoMemory;
			d3dDesc.pInstance = this;

			auto d3dAdapter = std::make_shared<D3D11Adapter>(d3dDesc, adapter);

			AddAdapter(d3dAdapter);
		}
	}
}
