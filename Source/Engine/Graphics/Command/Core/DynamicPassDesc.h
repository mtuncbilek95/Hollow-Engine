#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/Core/DynamicPassAttachmentDesc.h>

namespace Hollow
{
	struct DynamicPassDesc
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
