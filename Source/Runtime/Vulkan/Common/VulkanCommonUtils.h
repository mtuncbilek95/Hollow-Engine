#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Common/ShareMode.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommonUtils
	{
	public:
		static VkSharingMode GetVkSharingMode(ShareMode mode)
		{
			switch (mode)
			{
			case ShareMode::Exclusive:
				return VK_SHARING_MODE_EXCLUSIVE;
			case ShareMode::Concurrent:
				return VK_SHARING_MODE_CONCURRENT;
			default:
				return VK_SHARING_MODE_EXCLUSIVE;
			}
		}

	public:
		VulkanCommonUtils() = delete;
		~VulkanCommonUtils() = delete;
	};
}