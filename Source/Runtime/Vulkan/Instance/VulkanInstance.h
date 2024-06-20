#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanInstance final: public GraphicsInstance
	{
	public:
		VulkanInstance(const GraphicsInstanceDesc& desc);
		~VulkanInstance() override;

		VkInstance GetVkInstance() const { return mVkInstance; }

	protected:
		virtual void EnumerateAdaptersImpl() override;

	private:
		VkInstance mVkInstance;
		DArray<const char*> mInstanceLayers;

#if defined(HOLLOW_DEBUG)
		VkDebugUtilsMessengerEXT mVkDebugMessenger;
#endif
	};
}
