#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsView.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11GraphicsView : public GraphicsView
	{
		friend class D3D11Device;
	public:
		D3D11GraphicsView(const GraphicsViewDesc& desc, D3D11Device* pDevice);
		~D3D11GraphicsView() override = default;
		
		ComPtr<ID3D11Buffer>& GetD3DBuffer() { return mD3DBuffer; }

	private:
		ComPtr<ID3D11Buffer> mD3DBuffer;
	};
}
