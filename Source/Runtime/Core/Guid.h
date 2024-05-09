#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Guid final
	{
	public:
		Guid();
		Guid(uint32 a, uint16 b, uint16 c, uint64 d);
		Guid(const Guid& other);
		Guid(const string& str);

		uint32& GetA() { return mA; }
		uint16& GetB() { return mB; }
		uint16& GetC() { return mC; }
		uint64& GetD() { return mD; }

		string ToString();

	private:
		uint32 mA;
		uint16 mB;
		uint16 mC;
		uint64 mD;
	};
}
