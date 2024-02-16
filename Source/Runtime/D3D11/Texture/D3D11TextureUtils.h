#pragma once

#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>

#include <d3d11_4.h>

namespace Hollow
{
	class RUNTIME_API D3D11TextureUtils
	{
	public:
		static DXGI_FORMAT GetDXTextureFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::Unknown:				return DXGI_FORMAT_UNKNOWN;
			case TextureFormat::RGBA32_Typeless:		return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			case TextureFormat::RGBA32_Float:			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case TextureFormat::RGBA32_UInt:			return DXGI_FORMAT_R32G32B32A32_UINT;
			case TextureFormat::RGBA32_Int:				return DXGI_FORMAT_R32G32B32A32_SINT;

			case TextureFormat::RGB32_Typeless:			return DXGI_FORMAT_R32G32B32_TYPELESS;
			case TextureFormat::RGB32_Float:			return DXGI_FORMAT_R32G32B32_FLOAT;
			case TextureFormat::RGB32_UInt:				return DXGI_FORMAT_R32G32B32_UINT;
			case TextureFormat::RGB32_Int:				return DXGI_FORMAT_R32G32B32_SINT;

			case TextureFormat::RGBA16_Typeless:		return DXGI_FORMAT_R16G16B16A16_TYPELESS;
			case TextureFormat::RGBA16_Float:			return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case TextureFormat::RGBA16_UNorm:			return DXGI_FORMAT_R16G16B16A16_UNORM;
			case TextureFormat::RGBA16_UInt:			return DXGI_FORMAT_R16G16B16A16_UINT;
			case TextureFormat::RGBA16_Norm:			return DXGI_FORMAT_R16G16B16A16_SNORM;
			case TextureFormat::RGBA16_Int:				return DXGI_FORMAT_R16G16B16A16_SINT;

			case TextureFormat::RG32_Typeless:			return DXGI_FORMAT_R32G32_TYPELESS;
			case TextureFormat::RG32_Float:				return DXGI_FORMAT_R32G32_FLOAT;
			case TextureFormat::RG32_UInt:				return DXGI_FORMAT_R32G32_UINT;
			case TextureFormat::RG32_Int:				return DXGI_FORMAT_R32G32_SINT;

			case TextureFormat::RGBA8_Typeless:			return DXGI_FORMAT_R8G8B8A8_TYPELESS;
			case TextureFormat::RGBA8_UNorm:			return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8_UInt:				return DXGI_FORMAT_R8G8B8A8_UINT;
			case TextureFormat::RGBA8_Norm:				return DXGI_FORMAT_R8G8B8A8_SNORM;
			case TextureFormat::RGBA8_Int:				return DXGI_FORMAT_R8G8B8A8_SINT;

			case TextureFormat::RG16_Typeless:			return DXGI_FORMAT_R16G16_TYPELESS;
			case TextureFormat::RG16_Float:				return DXGI_FORMAT_R16G16_FLOAT;
			case TextureFormat::RG16_UNorm:				return DXGI_FORMAT_R16G16_UNORM;
			case TextureFormat::RG16_UInt:				return DXGI_FORMAT_R16G16_UINT;
			case TextureFormat::RG16_Norm:				return DXGI_FORMAT_R16G16_SNORM;
			case TextureFormat::RG16_Int:				return DXGI_FORMAT_R16G16_SINT;

			case TextureFormat::R32_Typeless:			return DXGI_FORMAT_R32_TYPELESS;
			case TextureFormat::D32_Float:				return DXGI_FORMAT_D32_FLOAT;
			case TextureFormat::R32_Float:				return DXGI_FORMAT_R32_FLOAT;
			case TextureFormat::R32_UInt:				return DXGI_FORMAT_R32_UINT;
			case TextureFormat::R32_Int:				return DXGI_FORMAT_R32_SINT;

			case TextureFormat::D24_UNorm_S8_UInt:		return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case TextureFormat::R24_UNorm_X8_Typeless:	return DXGI_FORMAT_R24G8_TYPELESS;

			case TextureFormat::RG8_Typeless:			return DXGI_FORMAT_R8G8_TYPELESS;
			case TextureFormat::RG8_UNorm:				return DXGI_FORMAT_R8G8_UNORM;
			case TextureFormat::RG8_UInt:				return DXGI_FORMAT_R8G8_UINT;
			case TextureFormat::RG8_Norm:				return DXGI_FORMAT_R8G8_SNORM;
			case TextureFormat::RG8_Int:				return DXGI_FORMAT_R8G8_SINT;

			case TextureFormat::R16_Typeless:			return DXGI_FORMAT_R16_TYPELESS;
			case TextureFormat::R16_Float:				return DXGI_FORMAT_R16_FLOAT;
			case TextureFormat::D16_UNorm:				return DXGI_FORMAT_D16_UNORM;
			case TextureFormat::R16_UNorm:				return DXGI_FORMAT_R16_UNORM;
			case TextureFormat::R16_UInt:				return DXGI_FORMAT_R16_UINT;
			case TextureFormat::R16_Norm:				return DXGI_FORMAT_R16_SNORM;
			case TextureFormat::R16_Int:				return DXGI_FORMAT_R16_SINT;

			case TextureFormat::R8_Typeless:			return DXGI_FORMAT_R8_TYPELESS;
			case TextureFormat::R8_UNorm:				return DXGI_FORMAT_R8_UNORM;
			case TextureFormat::R8_UInt:				return DXGI_FORMAT_R8_UINT;
			case TextureFormat::R8_Norm:				return DXGI_FORMAT_R8_SNORM;
			case TextureFormat::R8_Int:					return DXGI_FORMAT_R8_SINT;
			case TextureFormat::A8_UNorm:				return DXGI_FORMAT_A8_UNORM;
			default: 									return DXGI_FORMAT_UNKNOWN;
			}
		}

		static D3D11_BIND_FLAG GetDXTextureUsage(TextureUsage usage)
		{
			switch (usage)
			{
			case TextureUsage::Unknown:					return D3D11_BIND_RENDER_TARGET;
			case TextureUsage::RenderTarget:			return D3D11_BIND_RENDER_TARGET;
			case TextureUsage::ShaderResource:			return D3D11_BIND_SHADER_RESOURCE;
			case TextureUsage::DepthStencil:			return D3D11_BIND_DEPTH_STENCIL;
			case TextureUsage::UnorderedAccess:			return D3D11_BIND_UNORDERED_ACCESS;
			default:									return D3D11_BIND_RENDER_TARGET;
			}
		}
	public:
		D3D11TextureUtils() = delete;
		~D3D11TextureUtils() = delete;
	};
}
