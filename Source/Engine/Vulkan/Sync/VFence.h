#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sync/Fence.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VFence : public Fence
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
