#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

#include <Runtime/Graphics/GraphicsManager.h>

namespace Hollow
{
	class RUNTIME_API GraphicsQueue : public GraphicsDeviceObject
	{
	public:
		GraphicsQueue(const GraphicsQueueDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mQueueType(desc.QueueType) {}
		virtual ~GraphicsQueue() override = default;

		GraphicsQueueType GetGraphicsQueueType() const noexcept { return mQueueType; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Queue; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsQueueType mQueueType;
	};
}