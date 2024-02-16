#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Adapter : public GraphicsAdapter
	{
	public:
		D3D11Adapter(const GraphicsAdapterDesc& desc, ComPtr<IDXGIAdapter1> adapter);
		~D3D11Adapter() override = default;

		const ComPtr<IDXGIAdapter1>& GetD3DAdapter() const { return mD3DAdapter; }

	protected:
		virtual SharedPtr<GraphicsDevice> CreateDeviceCore(const GraphicsDeviceDesc& desc) override;
	private:
		ComPtr<IDXGIAdapter1> mD3DAdapter;
	};
}
