#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendType.h>
#include <Runtime/Graphics/Pipeline/BlendState/ColorMask.h>

namespace Hollow
{
	struct RUNTIME_API RenderTargetBlendDesc
	{
		bool BlendEnable;
		BlendType SrcBlend;
		BlendType DestBlend;
		BlendOperation BlendOp;
		BlendType SrcBlendAlpha;
		BlendType DestBlendAlpha;
		BlendOperation BlendOpAlpha;
		ColorMask ColorWriteMask;
	};
}
