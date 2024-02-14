#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Pipeline/DepthStencil/DepthMask.h>
#include <Runtime/Graphics/Pipeline/DepthStencil/DepthStencilComparisonFunc.h>

namespace Hollow
{
	struct RUNTIME_API DepthStencilDesc
	{
		bool DepthEnable;
		DepthMask DepthMask;
		ComparisonFunc DepthComparisonFunc;
		bool StencilEnable;
		byte StencilReadMask;
		byte StencilWriteMask;
	};
}
