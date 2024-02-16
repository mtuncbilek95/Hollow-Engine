#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Buffer/BufferCPUAccess.h>
#include <Runtime/Graphics/Buffer/BufferUsage.h>
#include <Runtime/Graphics/Buffer/ResourceUsageType.h>

#include <d3d11.h>

namespace Hollow
{
	class RUNTIME_API D3D11BufferUtils
	{
	public:
		static D3D11_BIND_FLAG GetBufferUsage(BufferUsage usage)
		{
			switch (usage)
			{
			case BufferUsage::Unkown:					return  D3D11_BIND_VERTEX_BUFFER;
			case BufferUsage::VertexBuffer:				return  D3D11_BIND_VERTEX_BUFFER;
			case BufferUsage::IndexBuffer:				return D3D11_BIND_INDEX_BUFFER;
			case BufferUsage::ConstantBuffer:			return D3D11_BIND_CONSTANT_BUFFER;
			default:									return D3D11_BIND_VERTEX_BUFFER;
			}
		}

		static uint32 GetBufferCPUAccess(BufferCPUAccess access)
		{
			switch (access)
			{
			case BufferCPUAccess::None:			return 0;
			case BufferCPUAccess::Read:			return D3D11_CPU_ACCESS_READ;
			case BufferCPUAccess::Write:		return D3D11_CPU_ACCESS_WRITE;
			default:							return 0;
			}
		}

		static D3D11_USAGE GetResourceUsage(ResourceUsageType usage)
		{
			switch (usage)
			{
			case ResourceUsageType::Default:			return D3D11_USAGE_DEFAULT;
			case ResourceUsageType::Immutable:			return D3D11_USAGE_IMMUTABLE;
			case ResourceUsageType::Dynamic:			return D3D11_USAGE_DYNAMIC;
			case ResourceUsageType::Staging:			return D3D11_USAGE_STAGING;
			default:									return D3D11_USAGE_DEFAULT;
			}
		}

	public:
		D3D11BufferUtils() = delete;
		~D3D11BufferUtils() = delete;
	};
}
