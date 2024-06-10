#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API ShaderBlockMember
	{
		String Name;
		u32 Offset;
		u32 AbsoluteOffset;
		u32 Size;
		u32 PaddedSize;
		ArrayList<ShaderBlockMember> Members;
	};
}
