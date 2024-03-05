#include "Queue.h"

namespace MiniVk
{
	Queue::Queue(const QueueDesc& desc, Renderer* pRenderer) :
		mQueueFamilyIndex(desc.QueueFamilyIndex), mType(desc.Type)
	{
		mVkLogicalDevice = pRenderer->GetVkLogicalDevice();
		switch (desc.Type)
		{
		case QueueType::Graphics:
			mQueue = pRenderer->GetFreeGraphicsQueue();
			break;
		case QueueType::Compute:
			mQueue = pRenderer->GetFreeComputeQueue();
			break;
		case QueueType::Transfer:
			mQueue = pRenderer->GetFreeTransferQueue();
			break;
		case QueueType::Present:
			mQueue = pRenderer->GetFreeGraphicsQueue();
			break;
		default:
			DEV_ASSERT(false, "Queue", "Queue type not supported");
			break;
		}
	}

	Queue::~Queue()
	{
	}
}