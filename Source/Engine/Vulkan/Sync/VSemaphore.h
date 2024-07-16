#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sync/Semaphore.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VSemaphore : public Semaphore
	{
	public:
		VSemaphore(SharedPtr<VDevice> pDevice);
		~VSemaphore() override = default;

		VkSemaphore GetVkSemaphore() const { return mSemaphore; }

	private:
		VkSemaphore mSemaphore;
		VkDevice mDevice;
	};
}
