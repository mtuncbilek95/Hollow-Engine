#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPassAttachmentDesc.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassDesc
	{
		Vector2u TargetSize;
		arrayList<RenderPassAttachmentDesc> Attachments;
		RenderPassAttachmentDesc DepthStencilAttachment;
	};
}