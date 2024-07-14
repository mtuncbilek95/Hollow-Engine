#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Instance/GraphicsInstance.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VInstance : public GraphicsInstance
	{
	public:
		VInstance(const GraphicsInstanceDesc& desc);
		~VInstance() override = default;

		VkInstance GetVkInstance() const { return mVkInstance; }
		VkPhysicalDevice GetVkAdapter() const { return mVkAdapter; }

	protected:
		VkPhysicalDevice GetBestAdapter();

	private:
		VkInstance mVkInstance;
		VkPhysicalDevice mVkAdapter;

#if defined(HOLLOW_DEBUG)
		VkDebugUtilsMessengerEXT mVkDebugger;
#endif
	};
}
