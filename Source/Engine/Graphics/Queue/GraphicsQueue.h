#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Queue/GraphicsQueueDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsQueue : public DeviceObject
	{
	public:
		GraphicsQueue(const GraphicsQueueDesc& desc, SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice), 
			mQueueType(desc.QueueType), mQueueIndex(desc.QueueIndex) {}
		virtual ~GraphicsQueue() override = default;

		GraphicsQueueType GetQueueType() const { return mQueueType; }
		u32 GetQueueIndex() const { return mQueueIndex; }
	private:
		GraphicsQueueType mQueueType;
		u32 mQueueIndex;
	};
}
