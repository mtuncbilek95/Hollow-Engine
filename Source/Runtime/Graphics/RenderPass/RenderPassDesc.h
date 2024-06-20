#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/PassAttachmentDesc.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassDesc
	{
		DArray<PassAttachmentDesc> ColorAttachments;
		DArray<PassAttachmentDesc> InputAttachments;
		PassAttachmentDesc ResolveAttachment;
		PassAttachmentDesc DepthStencilAttachment;
		PipelineStageFlags PipelineStageFlags;
		bool bHasDepthStencilAttachment;
		bool bHasResolveAttachment;
	};
}
