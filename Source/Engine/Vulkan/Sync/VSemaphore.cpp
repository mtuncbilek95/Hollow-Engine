#include "VSemaphore.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace Hollow
{
	VSemaphore::VSemaphore(SharedPtr<VDevice> pDevice) : Semaphore(pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		CORE_ASSERT(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mSemaphore) == VK_SUCCESS, "VSemaphore", "Failed to create semaphore!");
	}
}
