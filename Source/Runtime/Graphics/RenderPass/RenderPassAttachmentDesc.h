#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		SharedPtr<Texture> pTexture;
		byte MipLevel;
		byte ArrayLevel;
	};
}