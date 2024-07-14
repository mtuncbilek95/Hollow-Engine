#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/Shader.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VShader : public Shader
	{
	public:
		VShader(const ShaderDesc& desc, SharedPtr<VDevice> pDevice);
		~VShader() override = default;

		VkShaderModule GetShaderModule() const { return mShaderModule; }

	private:
		VkShaderModule mShaderModule;
		VkDevice mDevice;
	};
}
