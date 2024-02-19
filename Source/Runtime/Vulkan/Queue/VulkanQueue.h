#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>
namespace Hollow
{
	class RUNTIME_API VulkanQueue : public GraphicsQueue
	{
	public:
		VulkanQueue(const GraphicsQueueDesc& desc, VulkanDevice* pDevice);
		~VulkanQueue() override = default;

		const VkQueue& GetVkQueue() const noexcept { return mVkQueue; }
		uint32 GetQueueFamilyIndex() const noexcept { return mQueueFamilyIndex; }

		void OnShutdown() noexcept override;
	
	private:
		VkQueue mVkQueue;
		uint32 mQueueFamilyIndex;
	};
}
