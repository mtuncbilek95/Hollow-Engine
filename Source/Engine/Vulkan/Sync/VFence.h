#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sync/Fence.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VFence : public Fence
	{
	public:
		VFence(bool bSignalled, SharedPtr<VDevice> pDevice);
		~VFence() override = default;

		VkFence GetVkFence() const { return mFence; }

	private:
		VkFence mFence;
		VkDevice mDevice;
	};
}
