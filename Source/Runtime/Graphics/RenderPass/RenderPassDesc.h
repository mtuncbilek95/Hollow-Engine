#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/PassAttachmentDesc.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassDesc
	{
		ArrayList<PassAttachmentDesc> ColorAttachments;
		ArrayList<PassAttachmentDesc> InputAttachments;
		PassAttachmentDesc ResolveAttachment;
		PassAttachmentDesc DepthStencilAttachment;
		PipelineStageFlags PipelineStageFlags;
		bool bHasDepthStencilAttachment;
		bool bHasResolveAttachment;
	};
}
