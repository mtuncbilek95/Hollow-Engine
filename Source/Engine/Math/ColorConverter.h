#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	namespace Color
	{
		// Convert Hex to RGB
		static Vec4f HexToRGB(const String& hexString)
		{
			Vec4f color;
			color.x = std::stoul(hexString.substr(1, 2), nullptr, 16) / 255.0f;
			color.y = std::stoul(hexString.substr(3, 2), nullptr, 16) / 255.0f;
			color.z = std::stoul(hexString.substr(5, 2), nullptr, 16) / 255.0f;
			color.w = 1.0f;
			return color;
		}

		// Convert RGB to Hex
		static String RGBToHex(const Vec4f& color)
		{
			String hexString = "#";
			hexString += std::to_string(static_cast<int>(color.x * 255));
			hexString += std::to_string(static_cast<int>(color.y * 255));
			hexString += std::to_string(static_cast<int>(color.z * 255));
			return hexString;
		}
	}
}
