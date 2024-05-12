#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API ShaderBlockMember
	{
		String Name;
		uint32 Offset;
		uint32 AbsoluteOffset;
		uint32 Size;
		uint32 PaddedSize;
		ArrayList<ShaderBlockMember> Members;
	};
}
