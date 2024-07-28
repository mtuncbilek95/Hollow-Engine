#pragma once

#include <Engine/Core/Core.h>

#include <ostream>
#include <iomanip>
#include <sstream>

namespace Hollow
{
	namespace Color
	{
		// Convert Hex to RGB
		static Vec4f HexToRGB_F(const String& hexString)
		{
			Vec4f color;
			color.x = std::stoul(hexString.substr(1, 2), nullptr, 16) / 255.0f;
			color.y = std::stoul(hexString.substr(3, 2), nullptr, 16) / 255.0f;
			color.z = std::stoul(hexString.substr(5, 2), nullptr, 16) / 255.0f;
			color.w = 1.0f;
			return color;
		}

		// Convert RGB to Hex
		static String RGB_FToHex(const Vec4f& color)
		{
			std::ostringstream hexStream;
			hexStream << "#"
				<< std::setfill('0') << std::setw(2) << std::hex << (int)(color.x * 255)
				<< std::setfill('0') << std::setw(2) << std::hex << (int)(color.y * 255)
				<< std::setfill('0') << std::setw(2) << std::hex << (int)(color.z * 255)
				<< std::setfill('0') << std::setw(2) << std::hex << (int)(color.w * 255);
			return hexStream.str();
		}

		static Vec4s HexToRGB_B(const String& hexString)
		{
			Vec4s color;
			color.x = static_cast<u8>(std::stoul(hexString.substr(1, 2), nullptr, 16));
			color.y = static_cast<u8>(std::stoul(hexString.substr(3, 2), nullptr, 16));
			color.z = static_cast<u8>(std::stoul(hexString.substr(5, 2), nullptr, 16));
			color.w = 255;
			return color;
		}
	}
}
