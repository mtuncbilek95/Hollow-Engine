#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Core/ResourceSubObject.h>
#include <Runtime/Graphics/Shader/Shader.h>

namespace Hollow
{
	class RUNTIME_API ShaderResource final : public ResourceSubObject
	{
	public:
		ShaderResource();
		virtual ~ShaderResource() override = default;

		void CompileShader();

		SharedPtr<Shader> GetShaderModule() const noexcept { return mShaderModule; }


		virtual void OnShutdown() noexcept override;
		virtual ResourceObjectType GetObjectType() const noexcept final { return ResourceObjectType::Shader; }

	private:
		SharedPtr<Shader> mShaderModule;
	};
}
