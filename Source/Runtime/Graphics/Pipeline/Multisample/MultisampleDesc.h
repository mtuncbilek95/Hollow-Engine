#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureSample.h>

namespace Hollow
{
	struct RUNTIME_API MultisampleDesc
	{
		bool bEnable;
		TextureSample SampleCount;
	};
}