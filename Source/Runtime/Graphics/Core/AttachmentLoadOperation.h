#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API AttachmentLoadOperation : byte
	{
		Load,
		Clear,
		DontCare
	};
}
