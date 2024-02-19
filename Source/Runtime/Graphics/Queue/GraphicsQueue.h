#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsQueue : public GraphicsDeviceObject
	{
	public:
		GraphicsQueue(const GraphicsQueueDesc& desc) : mQueueType(desc.Type) 
		{}
		virtual ~GraphicsQueue() override = default;

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Queue; }
		
		virtual void OnShutdown() noexcept override = 0;

		GraphicsQueueType GetQueueType() const noexcept { return mQueueType; }

	private:
		GraphicsQueueType mQueueType;
	};
}
