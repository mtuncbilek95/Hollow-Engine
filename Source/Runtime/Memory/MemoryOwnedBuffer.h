#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API MemoryOwnedBuffer final
	{
	public:
		MemoryOwnedBuffer();
		MemoryOwnedBuffer(void* pData, uint64 size);
		MemoryOwnedBuffer(void* pStart, void* pEnd);
		MemoryOwnedBuffer(const MemoryOwnedBuffer& other);
		~MemoryOwnedBuffer();

		FORCEINLINE void* GetBuffer() const { return mBuffer; }
		FORCEINLINE uint64 GetSize() const { return mSize; }

	private:
		void* mBuffer;
		uint64 mSize;
	};
}
