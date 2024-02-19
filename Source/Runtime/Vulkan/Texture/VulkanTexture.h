#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTexture : public Texture
	{
	public:
		VulkanTexture(const TextureDesc& desc);
		~VulkanTexture() override = default;

		const VkImage& GetVkImage() const { return mVkImage; }

	private:
		VkImage mVkImage;
	};
}
