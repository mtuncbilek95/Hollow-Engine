#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Guid final
	{
	public:
		Guid();
		Guid(u32 a, u16 b, u16 c, u64 d);
		Guid(const Guid& other);
		Guid(const String& str);

		u32& GetA() { return mA; }
		u16& GetB() { return mB; }
		u16& GetC() { return mC; }
		u64& GetD() { return mD; }

		String ToString();

	private:
		u32 mA;
		u16 mB;
		u16 mC;
		u64 mD;
	};
}
