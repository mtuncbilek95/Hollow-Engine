#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Texture/TextureResourceLayout.h>

namespace Hollow
{
	class RUNTIME_API TextureImporter
	{
	public:
		static SharedPtr<TextureResourceLayout> ImportTexture(const String& path);

	public:
		TextureImporter() = delete;
		~TextureImporter() = delete;
	};
}
