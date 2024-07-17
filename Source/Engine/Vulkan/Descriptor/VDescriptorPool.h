#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorPool.h>
#include <Engine/Vulkan/Device/VDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDescriptorPool final : public DescriptorPool
	{
	public:
		VDescriptorPool(const DescriptorPoolDesc& desc, const WeakPtr<VDevice>& pDevice);
		~VDescriptorPool() override = default;

		VkDescriptorPool GetVkDescriptorPool() const { return mVkDescriptorPool; }
		VkDevice GetVkDevice() const { return mDevice; }

	private:
		VkDescriptorPool mVkDescriptorPool;
		VkDevice mDevice;
	};
}
