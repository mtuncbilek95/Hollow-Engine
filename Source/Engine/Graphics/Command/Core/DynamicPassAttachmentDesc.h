#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureLayout.h>
#include <Engine/Graphics/Command/Core/ResolveModeFlags.h>
#include <Engine/Graphics/Command/Core/AttachmentLoadOperation.h>
#include <Engine/Graphics/Command/Core/AttachmentStoreOperation.h>

namespace Hollow
{
	class TextureView;
	struct DynamicPassAttachmentDesc
	{
		WeakPtr<TextureView> ImageBuffer;
		TextureLayout ImageLayout;
		ResolveModeFlags ResolveMode;
		WeakPtr<TextureView> ResolveBuffer;
		TextureLayout ResolveLayout;
		AttachmentLoadOperation LoadOperation;
		AttachmentStoreOperation StoreOperation;
		Vec4f ClearColor;
		Vec2f ClearDepthStencil;
	};
}
