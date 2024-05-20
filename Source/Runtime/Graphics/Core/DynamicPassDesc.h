#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Core/DynamicPassAttachmentDesc.h>

namespace Hollow
{
	struct RUNTIME_API DynamicPassDesc
	{
		Vector2u Extent;
		Vector2i Offset;
		uint32 layerCount;
		uint32 viewMask;
		ArrayList<DynamicPassAttachmentDesc> ColorAttachments;
		DynamicPassAttachmentDesc DepthAttachment;
		DynamicPassAttachmentDesc StencilAttachment;
	};
}
