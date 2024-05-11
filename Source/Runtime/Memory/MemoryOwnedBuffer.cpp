#include "MemoryOwnedBuffer.h"

namespace Hollow
{
	MemoryOwnedBuffer::MemoryOwnedBuffer() : mBuffer(nullptr), mSize(0)
	{
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(void* pData, uint64 size) : mBuffer(pData), mSize(size)
	{
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(void* pStart, void* pEnd) : mBuffer(pStart), mSize((byte*)pEnd - (byte*)pStart)
	{
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(const MemoryOwnedBuffer& other) : mBuffer(other.mBuffer), mSize(other.mSize)
	{
	}

	MemoryOwnedBuffer::~MemoryOwnedBuffer()
	{
	}
}
