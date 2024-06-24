#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Core/DynamicPassAttachmentDesc.h>

namespace Hollow
{
	struct RUNTIME_API DynamicPassDesc
	{
		Vec2u Extent;
		Vec2i Offset;
		u32 layerCount;
		u32 viewMask;
		DArray<DynamicPassAttachmentDesc> ColorAttachments;
		DynamicPassAttachmentDesc DepthAttachment;
		DynamicPassAttachmentDesc StencilAttachment;
	};
}
