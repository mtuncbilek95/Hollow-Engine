#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsQueue : public GraphicsDeviceObject
	{
	public:
		GraphicsQueue(const GraphicsQueueDesc& desc, SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mQueueType(desc.QueueType) {}
		virtual ~GraphicsQueue() override = default;

		FORCEINLINE GraphicsQueueType GetQueueType() const noexcept { return mQueueType; }
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Queue; }

	private:
		GraphicsQueueType mQueueType;
	};
}
