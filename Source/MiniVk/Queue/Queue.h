#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	enum class QueueType
	{
		Graphics,
		Compute,
		Transfer,
		Present
	};

	struct QueueDesc
	{
		uint32 QueueFamilyIndex;
		QueueType Type;
	};

	class Queue
	{
	public:
		Queue(const QueueDesc& desc, Renderer* pRenderer);
		~Queue();

		VkQueue GetQueue() const { return mQueue; }
		uint32 GetQueueFamilyIndex() const { return mQueueFamilyIndex; }
		QueueType GetType() const { return mType; }

	private:
		VkQueue mQueue;
		VkDevice mVkLogicalDevice;
		uint32 mQueueFamilyIndex;
		QueueType mType;
	};
}