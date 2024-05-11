#include "TextureImporter.h"

#if defined(STB_IMAGE_IMPLEMENTATION)
#include <stb_image.h>
#endif

namespace Hollow
{
	SharedPtr<TextureResourceLayout> TextureImporter::ImportTexture(const String& path)
	{
		int width, height, channels;
		byte* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		auto layout = std::make_shared<TextureResourceLayout>();
		layout->ImageSize = Vector2i(width, height);
		layout->ImageData = MemoryBuffer(data, width * height * STBI_rgb_alpha);
		layout->Channels = channels;

		return layout;
	}
}
