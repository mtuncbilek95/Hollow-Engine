#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Buffer/GraphicsBuffer.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VBuffer : public GraphicsBuffer
	{
	public:
		VBuffer(const GraphicsBufferDesc& desc, SharedPtr<VDevice> pDevice);
		~VBuffer() override = default;

		VkBuffer GetBuffer() const { return mBuffer; }

		u64 GetOffset() const { return mOffset; }
		u64 GetAlignedOffset() const { return mAlignedOffset; }

	private:
		VkBuffer mBuffer;
		VkDevice mDevice;

		u64 mOffset;
		u64 mAlignedOffset;
	};
}