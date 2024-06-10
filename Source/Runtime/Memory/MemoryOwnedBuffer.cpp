#include "MemoryOwnedBuffer.h"

namespace Hollow
{
	MemoryOwnedBuffer::MemoryOwnedBuffer() : mData(nullptr), mSize(0)
	{
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(void* pData, const u64 size) : mData(nullptr), mSize(0)
	{
		AllocateAndCopy(pData, size);
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(const MemoryOwnedBuffer& other) : mData(nullptr), mSize(0)
	{
		AllocateAndCopy(other.mData, other.mSize);
	}

	MemoryOwnedBuffer::MemoryOwnedBuffer(MemoryOwnedBuffer&& other) noexcept : mData(other.mData), mSize(other.mSize)
	{
		other.mData = nullptr;
		other.mSize = 0;
	}

	MemoryOwnedBuffer::~MemoryOwnedBuffer()
	{
		delete[] static_cast<i8*>(mData);
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator=(const MemoryOwnedBuffer& other)
	{
		if (this != &other)
		{
			void* newData = nullptr;
			if (other.mData && other.mSize > 0)
			{
				newData = new char[other.mSize];
				std::memcpy(newData, other.mData, other.mSize);
			}

			delete[] static_cast<char*>(mData);

			mData = newData;
			mSize = other.mSize;
		}
		return *this;
	}

	MemoryOwnedBuffer& MemoryOwnedBuffer::operator=(MemoryOwnedBuffer&& other) noexcept
	{
		if (this != &other)
		{
			delete[] static_cast<char*>(mData);

			mData = other.mData;
			mSize = other.mSize;

			other.mData = nullptr;
			other.mSize = 0;
		}
		return *this;
	}

	void MemoryOwnedBuffer::AllocateAndCopy(const void* pData, const u64 size)
	{
		if (pData && size > 0)
		{
			mData = new char[size];
			std::memcpy(mData, pData, size);
			mSize = size;
		}
	}
}
