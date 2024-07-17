#pragma once
#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemory.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VMemory : public GraphicsMemory
	{
	public:
		VMemory(const GraphicsMemoryDesc& desc, WeakPtr<VDevice> pDevice);
		~VMemory() override = default;

		VkDeviceMemory GetVkDeviceMemory() const { return mVkDeviceMemory; }

	protected:
		u32 FindMemoryType(GraphicsMemoryType memoryType);

	private:
		VkDeviceMemory mVkDeviceMemory;
		VkPhysicalDevice mVkAdapter;
		VkDevice mDevice;
	};
}
