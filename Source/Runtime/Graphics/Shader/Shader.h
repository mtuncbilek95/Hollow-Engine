#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>

namespace Hollow
{
	class RUNTIME_API Shader : public GraphicsDeviceObject
	{
	public:
		Shader(const ShaderDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice),
			mSource(desc.Source), mStage(desc.Stage),
			mEntryPoint(desc.EntryPoint), mShaderLanguage(desc.Language), mShaderName(desc.ShaderName)
		{}
		virtual ~Shader() override = default;

		const String& GetShaderName() const { return mShaderName; }
		const String& GetSource() const { return mSource; }
		ShaderStage GetStage() const { return mStage; }
		ShaderLanguage GetShaderLanguage() const { return mShaderLanguage; }
		const String& GetEntryPoint() const { return mEntryPoint; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Shader; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		String mShaderName;
		String mSource;
		ShaderStage mStage;
		ShaderLanguage mShaderLanguage;
		String mEntryPoint;
	};
}