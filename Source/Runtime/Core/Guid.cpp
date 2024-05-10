#include "Guid.h"

#include <format>

namespace Hollow
{
	Guid::Guid() : mA(0), mB(0), mC(0), mD(0)
	{
	}

	Guid::Guid(uint32 a, uint16 b, uint16 c, uint64 d) : mA(a), mB(b), mC(c), mD(d)
	{
	}

	Guid::Guid(const Guid& other) : mA(other.mA), mB(other.mB), mC(other.mC), mD(other.mD)
	{
	}

	Guid::Guid(const String& str) : mA(0), mB(0), mC(0), mD(0)
	{
		uint16 d_first;
		uint64 d_second;

		int consumed = 0;

		int result = sscanf(str.c_str(), "%8x-%4hx-%4hx-%4hx-%12llx%n", &mA, &mB, &mC, &d_first, &d_second, &consumed);

		mD = (uint64)d_first << 48 | d_second;

		if (consumed != str.size() || result != 5)
		{
			mA = 0;
			mB = 0;
			mC = 0;
			mD = 0;
		}
	}

	String Guid::ToString()
	{
		char buffer[64];
		sprintf(buffer, "%08x-%04hx-%04hx-%04hx-%012llx", mA, mB, mC, (uint16)(mD >> 48), mD & 0x0000FFFFFFFFFFFF);
		return buffer;
	}
}
