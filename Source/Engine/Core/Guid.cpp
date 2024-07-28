#include "Guid.h"

#include <format>

namespace Hollow
{
	Guid::Guid() : mA(0), mB(0), mC(0), mD(0)
	{
	}

	Guid::Guid(u32 a, u16 b, u16 c, u64 d) : mA(a), mB(b), mC(c), mD(d)
	{
	}

	Guid::Guid(const Guid& other) : mA(other.mA), mB(other.mB), mC(other.mC), mD(other.mD)
	{
	}

	Guid::Guid(const String& str) : mA(0), mB(0), mC(0), mD(0)
	{
		std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), mA, 16);
	}

	String Guid::ToString()
	{
		char buffer[64];
		std::snprintf(buffer, sizeof(buffer), "%08x-%04x-%04x-%04x-%012llx", mA, mB, mC, (u16)mD, mD >> 16);
		return buffer;
	}

	bool Guid::operator==(const Guid& other) const
	{
		return other.mA == mA && other.mB == mB && other.mC == mC && other.mD == mD;
	}

	bool Guid::operator!=(const Guid& other) const
	{
		return !(other == *this);
	}
}
