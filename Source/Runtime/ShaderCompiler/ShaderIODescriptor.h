#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>

namespace Hollow
{
	struct RUNTIME_API ShaderIODescriptor
	{
		String Name;
		TextureFormat FormatType;
	};
}
