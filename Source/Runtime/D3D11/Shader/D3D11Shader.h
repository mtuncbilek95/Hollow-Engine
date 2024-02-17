#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const ShaderDesc& desc, D3D11Device* pDevice);
		virtual ~D3D11Shader() override = default;

		const ComPtr<ID3D11VertexShader>& GetD3DVertexShader() const { return mD3DVertexShader; }
		const ComPtr<ID3D11PixelShader>& GetD3DPixelShader() const { return mD3DPixelShader; }
		const ComPtr<ID3D11GeometryShader>& GetD3DGeometryShader() const { return mD3DGeometryShader; }
		const ComPtr<ID3D11HullShader>& GetD3DHullShader() const { return mD3DHullShader; }
		const ComPtr<ID3D11DomainShader>& GetD3DDomainShader() const { return mD3DDomainShader; }
		const ComPtr<ID3D11ComputeShader>& GetD3DComputeShader() const { return mD3DComputeShader; }

		const ComPtr<ID3DBlob>& GetD3DBlob() const { return mShaderBlob; }

	private:
		ComPtr<ID3D11VertexShader> mD3DVertexShader;
		ComPtr<ID3D11PixelShader> mD3DPixelShader;
		ComPtr<ID3D11GeometryShader> mD3DGeometryShader;
		ComPtr<ID3D11HullShader> mD3DHullShader;
		ComPtr<ID3D11DomainShader> mD3DDomainShader;
		ComPtr<ID3D11ComputeShader> mD3DComputeShader;

		ComPtr<ID3DBlob> mShaderBlob;
	};
}
