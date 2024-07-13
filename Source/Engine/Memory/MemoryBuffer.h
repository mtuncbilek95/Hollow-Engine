#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryOwnedBuffer.h>

namespace Hollow
{
	class ENGINE_API MemoryBuffer final
	{
	public:
		MemoryBuffer();
		MemoryBuffer(void* pData, const u64 size);
		MemoryBuffer(const MemoryBuffer& other);
		MemoryBuffer(const MemoryOwnedBuffer& pOwned);
		~MemoryBuffer() = default;

		MemoryBuffer& operator=(const MemoryBuffer& other) noexcept
		{
			mBuffer = other.mBuffer;
			mSize = other.mSize;

			return *this;
		}

		MemoryBuffer& operator=(const MemoryOwnedBuffer& other) noexcept
		{
			mBuffer = other.GetData();
			mSize = other.GetSize();

			return *this;
		}

		FORCEINLINE void* GetData() const { return mBuffer; }
		FORCEINLINE u64 GetSize() const { return mSize; }

	private:
		void* mBuffer;
		u64 mSize;
	};
}
