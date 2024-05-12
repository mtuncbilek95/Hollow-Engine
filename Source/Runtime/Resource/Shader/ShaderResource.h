#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderReflection.h>

namespace Hollow
{
	class RUNTIME_API ShaderResource final : public ResourceSubObject
	{
	public:
		ShaderResource();
		virtual ~ShaderResource() override = default;

		void CompileShader(const String path, const String shaderName, const String EntryPoint,const ShaderStage stage,const ShaderLanguage lang);

		SharedPtr<Shader> GetShaderModule() const noexcept { return mShaderModule; }
		SharedPtr<ShaderReflection> GetReflection() const noexcept { return mReflection; }

		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Shader; }

	private:
		SharedPtr<GraphicsDevice> mGraphicsDevice;
		SharedPtr<Shader> mShaderModule;
		SharedPtr<ShaderReflection> mReflection;
	};
}
