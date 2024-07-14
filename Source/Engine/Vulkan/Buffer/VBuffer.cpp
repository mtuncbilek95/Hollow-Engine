#include "VBuffer.h"
#include "VBufferUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Memory/VMemory.h>

namespace Hollow
{
	VBuffer::VBuffer(const GraphicsBufferDesc& desc, SharedPtr<VDevice> pDevice) : GraphicsBuffer(desc, pDevice), mBuffer(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice())
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.BlockSize * desc.BlockLength;
		bufferInfo.usage = Utils::GetVKBufferUsage(desc.Usage);
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = 0;

		CORE_ASSERT(vkCreateBuffer(mDevice, &bufferInfo, nullptr, &mBuffer) == VK_SUCCESS, "VBuffer", "Failed to create buffer!");

		VkMemoryRequirements info;
		vkGetBufferMemoryRequirements(mDevice, mBuffer, &info);

		u64 memoryOffset = desc.pRequestMemory->Allocate(info.size + info.alignment);
		u64 alignedOffset = memoryOffset + (memoryOffset % info.alignment == 0 ? 0 : (info.alignment - (memoryOffset % info.alignment)));

		auto memPtr = desc.pRequestMemory->GetSharedPtrAs<VMemory>();
		CORE_ASSERT(vkBindBufferMemory(mDevice, mBuffer, memPtr->GetVkDeviceMemory(), alignedOffset) == VK_SUCCESS, "VBuffer", "Failed to bind buffer memory!");

		mOffset = memoryOffset;
		mAlignedOffset = alignedOffset;
	}
}
