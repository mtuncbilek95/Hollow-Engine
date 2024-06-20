#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>

namespace Hollow
{
	struct RUNTIME_API FramebufferDesc
	{
		SharedPtr<RenderPass> RPass;
		DArray<SharedPtr<TextureBuffer>> Attachments;
		Vector2u ImageSize;
	};
}
