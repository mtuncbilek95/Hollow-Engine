#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>

namespace Hollow
{
	class RUNTIME_API Shader : public GraphicsDeviceObject
	{
	public:
		Shader(const ShaderDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mSource(desc.Source), mStage(desc.Stage),
			mEntryPoint(desc.EntryPoint), mShaderLanguage(desc.Language), mShaderName(desc.ShaderName)
		{}
		virtual ~Shader() override = default;

		const string& GetShaderName() const { return mShaderName; }
		const string& GetSource() const { return mSource; }
		ShaderStage GetStage() const { return mStage; }
		ShaderLanguage GetShaderLanguage() const { return mShaderLanguage; }
		const string& GetEntryPoint() const { return mEntryPoint; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Shader; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		string mShaderName;
		string mSource;
		ShaderStage mStage;
		ShaderLanguage mShaderLanguage;
		string mEntryPoint;
	};
}