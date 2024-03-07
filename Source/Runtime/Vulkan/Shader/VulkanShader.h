#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanShader : public Shader
	{
	public:
		VulkanShader(const ShaderDesc& desc, VulkanDevice* pDevice);
		~VulkanShader() override = default;

		VkShaderModule GetVkShaderModule() const noexcept { return mVkShaderModule; }
		VkDevice GetVkDevice() const noexcept { return mVkDevice; }

		// Inherited via Shader
		void OnShutdown() noexcept override;

	private:
		VkShaderModule mVkShaderModule;
		VkDevice mVkDevice;

		uint32* mShaderSPIRV;
	};
}