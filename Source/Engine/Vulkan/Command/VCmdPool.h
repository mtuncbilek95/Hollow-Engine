#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CmdPool.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VCmdPool : public CmdPool
	{
	public:
		VCmdPool(const CmdPoolDesc& desc, const WeakPtr<VDevice> pDevice);
		~VCmdPool() override = default;

		VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }

	private:
		VkCommandPool mVkCommandPool;
		VkDevice mDevice;
	};
}
