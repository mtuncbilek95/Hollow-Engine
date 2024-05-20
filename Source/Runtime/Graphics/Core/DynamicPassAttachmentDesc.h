#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Core/ResolveModeFlags.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>
#include <Runtime/Graphics/Core/AttachmentLoadOperation.h>
#include <Runtime/Graphics/Core/AttachmentStoreOperation.h>

namespace Hollow
{
	struct RUNTIME_API DynamicPassAttachmentDesc
	{
		SharedPtr<TextureBuffer> ImageBuffer;
		TextureMemoryLayout TextureLayout;
		ResolveModeFlags ResolveMode;
		SharedPtr<TextureBuffer> ResolveBuffer;
		TextureMemoryLayout ResolveLayout;
		AttachmentLoadOperation LoadOperation;
		AttachmentStoreOperation StoreOperation;
		Vector4f ClearColor;
		Vector2f ClearDepthStencil;
	};
}
