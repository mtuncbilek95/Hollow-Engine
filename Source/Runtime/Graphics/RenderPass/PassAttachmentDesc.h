#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Core/AttachmentLoadOperation.h>
#include <Runtime/Graphics/Core/AttachmentStoreOperation.h>

namespace Hollow
{
	struct RUNTIME_API PassAttachmentDesc
	{
		TextureFormat AttachmentFormat;
		TextureSampleCount SampleCount;
		AttachmentLoadOperation ColorLoadOp;
		AttachmentStoreOperation ColorStoreOp;
		AttachmentLoadOperation StencilLoadOp;
		AttachmentStoreOperation StencilStoreOp;
		TextureMemoryLayout InitialLayout;
		TextureMemoryLayout FinalLayout;


	};
}
