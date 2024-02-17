#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11RenderPass : public RenderPass
	{
	public:
		D3D11RenderPass(const RenderPassDesc& desc, D3D11Device* pDevice);
		~D3D11RenderPass() override = default;

		const ArrayList<ComPtr<ID3D11RenderTargetView>>& GetRenderTargetViews() const { return mD3DRenderTargetViews; }
		const ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const { return mD3DDepthStencilView; }
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return mD3DShaderResourceView; }
		const ComPtr<ID3D11UnorderedAccessView>& GetUnorderedAccessView() const { return mD3DUnorderedAccessView; }

	private:
		ArrayList<ComPtr<ID3D11RenderTargetView>> mD3DRenderTargetViews;
		ComPtr<ID3D11DepthStencilView> mD3DDepthStencilView;
		ComPtr<ID3D11ShaderResourceView> mD3DShaderResourceView;
		ComPtr<ID3D11UnorderedAccessView> mD3DUnorderedAccessView;
	};
}
