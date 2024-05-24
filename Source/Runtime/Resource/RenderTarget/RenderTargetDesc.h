#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Texture/Texture.h>

namespace Hollow
{
	struct RUNTIME_API RenderTargetDesc
	{
		Vector2u mRegionSize;
		struct
		{
			ArrayList<SharedPtr<Texture>> ColorAttachments;
			SharedPtr<Texture> DepthAttachment;
			SharedPtr<Texture> StencilAttachment;
		} AttachmentStruct;
		struct
		{
			uint32 ColorAttachmentCount;
			TextureDesc ColorAttachment;
			bool HasDepthAttachment;
			TextureDesc DepthAttachment;
			bool HasStencilAttachment;
			TextureDesc StencilAttachment;
		} DescStruct;
	};
}

