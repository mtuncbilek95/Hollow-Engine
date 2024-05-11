#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanShader final : public Shader
	{
	public:
		VulkanShader(const ShaderDesc& desc, SharedPtr<VulkanDevice> device);
		~VulkanShader() override = default;

		VkShaderModule GetVkShaderModule() const { return mVkShaderModule; }

		void OnShutdown() noexcept override;

	private:
		VkShaderModule mVkShaderModule;
		VkDevice mVkDevice;
	};
}
