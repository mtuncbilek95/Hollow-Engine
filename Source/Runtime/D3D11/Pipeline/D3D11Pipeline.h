#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>

#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Pipeline : public Pipeline
	{
	public:
		D3D11Pipeline(const GraphicsPipelineDesc& desc, ID3D11Device* pDevice);
		D3D11Pipeline(const ComputePipelineDesc& desc, ID3D11Device* pDevice);
		~D3D11Pipeline() override = default;

		const ComPtr<ID3D11InputLayout>& GetD3DInputLayout() const { return mD3DInputLayout; }
		const ComPtr<ID3D11RasterizerState>& GetD3DRasterizerState() const { return mD3DRasterizerState; }
		const ComPtr<ID3D11DepthStencilState>& GetD3DDepthStencilState() const { return mD3DDepthStencilState; }
		const ComPtr<ID3D11BlendState>& GetD3DBlendState() const { return mD3DBlendState; }
		D3D11_PRIMITIVE_TOPOLOGY GetD3DPrimitiveTopology();

	private:
		ComPtr<ID3D11InputLayout> mD3DInputLayout;
		ComPtr<ID3D11RasterizerState> mD3DRasterizerState;
		ComPtr<ID3D11DepthStencilState> mD3DDepthStencilState;
		ComPtr<ID3D11BlendState> mD3DBlendState;
	};
}
