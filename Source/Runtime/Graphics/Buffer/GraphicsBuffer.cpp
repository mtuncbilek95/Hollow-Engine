#include "GraphicsBuffer.h"

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>

namespace Hollow
{
	void GraphicsBuffer::AllocateMemory()
	{
		GraphicsMemoryDesc desc = {};
		desc.MemoryType = mMemoryType;
		desc.pOwnerBuffer = this;

		mMemory = GetOwnerDevice()->CreateGraphicsMemory(desc);
	}
}