#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Pipeline/BlendState/RenderTargetBlendDesc.h>

namespace Hollow
{
	struct RUNTIME_API BlendStateDesc
	{
		bool AlphaToCoverageEnable;
		bool IndependentBlendEnable;
		RenderTargetBlendDesc RenderTargetBlendDesc;
	};
}
