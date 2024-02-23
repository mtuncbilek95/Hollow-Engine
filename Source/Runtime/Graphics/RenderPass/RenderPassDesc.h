#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassAttachmentDesc.h>
#include <Runtime/Graphics/Texture/TextureView.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassDesc
	{
		Vector2u FramebufferSize;
		Array<RenderPassAttachmentDesc> ColorAttachments;
		bool HasDepthStencilAttachment;
		RenderPassAttachmentDesc DepthStencilAttachment;
	};
}
