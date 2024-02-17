#include "D3D11Shader.h"

#include <d3dcompiler.h>

namespace Hollow
{
	D3D11Shader::D3D11Shader(const ShaderDesc& desc, D3D11Device* pDevice) : Shader(desc)
	{
		ComPtr<ID3DBlob> errorBlob = {};

		D3DCompile(desc.Source.data(), desc.Source.length(), nullptr, nullptr, nullptr, desc.EntryPoint.c_str(),
			desc.ShaderModel.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, mShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob.Get() != nullptr && errorBlob->GetBufferPointer() != nullptr)
		{
			CORE_LOG(HE_ERROR, "Shader", "Failed to compile shader: %s", (char*)errorBlob->GetBufferPointer());
		}

		switch (desc.Type)
		{
		case ShaderType::Vertex:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateVertexShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DVertexShader.GetAddressOf())), "D3D11Shader",
				"Failed to create vertex shader %s", desc.ShaderName.c_str());
			break;
		}
		case ShaderType::Pixel:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreatePixelShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DPixelShader.GetAddressOf())), "D3D11Shader",
				"Failed to create pixel shader %s", desc.ShaderName.c_str());
			break;
		}
		case ShaderType::Geometry:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateGeometryShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DGeometryShader.GetAddressOf())), "D3D11Shader",
				"Failed to create geometry shader %s", desc.ShaderName.c_str());
			break;
		}
		case ShaderType::Domain:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateDomainShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DDomainShader.GetAddressOf())), "D3D11Shader",
				"Failed to create domain shader %s", desc.ShaderName.c_str());
			break;
		}
		case ShaderType::Hull:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateHullShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DHullShader.GetAddressOf())), "D3D11Shader",
				"Failed to create hull shader %s", desc.ShaderName.c_str());
			break;
		}
		case ShaderType::Compute:
		{
			DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateComputeShader(mShaderBlob->GetBufferPointer(), mShaderBlob->GetBufferSize(), nullptr, mD3DComputeShader.GetAddressOf())), "D3D11Shader",
				"Failed to create compute shader %s", desc.ShaderName.c_str());
			break;
		}
		default:
			CORE_LOG(HE_WARNING, "D3D11Shader", "Invalid shader type %d", desc.Type);
			break;
		}

		CORE_LOG(HE_VERBOSE, "D3D11Shader", "Operation is successful.");
	}
}
