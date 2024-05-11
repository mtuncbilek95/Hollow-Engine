#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API MemoryOwnedBuffer;

	class RUNTIME_API MemoryBuffer final
	{
	public:
		MemoryBuffer();
		MemoryBuffer(void* pData, const uint64 size);
		MemoryBuffer(const MemoryBuffer& other);
		MemoryBuffer(const SharedPtr<MemoryOwnedBuffer> pOwned);
		~MemoryBuffer() = default;

		MemoryBuffer& operator=(const MemoryBuffer& other)
		{
			mBuffer = other.mBuffer;
			mSize = other.mSize;

			return *this;
		}

		FORCEINLINE void* GetBuffer() const { return mBuffer; }
		FORCEINLINE uint64 GetSize() const { return mSize; }

	private:
		void* mBuffer;
		uint64 mSize;
	};
}
