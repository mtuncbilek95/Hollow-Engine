#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

namespace Hollow
{
	struct GraphicsInstanceDesc
	{
		String AppName;
		Vec3b AppVersion;

		String EngineName;
		Vec3b EngineVersion;
	};
}
