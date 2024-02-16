#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Instance : public GraphicsInstance
	{
	public:
		D3D11Instance(const GraphicsInstanceDesc& desc);
		~D3D11Instance() override = default;

		const ComPtr<IDXGIFactory1>& GetD3DInstance() const { return mD3DInstance; }

	private:
		ComPtr<IDXGIFactory1> mD3DInstance;
	};
}
