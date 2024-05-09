#include "MemoryOwnedBuffer.h"

namespace Hollow
{
	MemoryOwnedBuffer::MemoryOwnedBuffer() : mBuffer(nullptr), mSize(0)
	{
	}
	MemoryOwnedBuffer::MemoryOwnedBuffer(void* pData, uint64 size) : mBuffer(pData), mSize(size)
	{
	}
	MemoryOwnedBuffer::MemoryOwnedBuffer(void* pStart, void* pEnd) : mBuffer(pStart), mSize((uint64)pEnd - (uint64)pStart)
	{
	}
	MemoryOwnedBuffer::~MemoryOwnedBuffer()
	{
		delete mBuffer;
	}
}
