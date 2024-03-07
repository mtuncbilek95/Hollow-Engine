#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Hollow
{
	class RUNTIME_API VulkanQueue : public GraphicsQueue
	{
	public:
		VulkanQueue(const GraphicsQueueDesc& desc, VkQueue reservedQueue, VulkanDevice* pDevice);
		~VulkanQueue() override = default;

		VkQueue GetVkQueue() const { return mVkQueue; }
		uint32 GetQueueFamilyIndex() const { return mQueueFamilyIndex; }

		virtual void OnShutdown() noexcept override;

	private:
		VkQueue mVkQueue;
		uint32 mQueueFamilyIndex;
	};
}