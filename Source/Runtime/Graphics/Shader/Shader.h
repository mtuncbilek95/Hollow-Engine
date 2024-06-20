#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>

namespace Hollow
{
	class RUNTIME_API Shader : public GraphicsDeviceObject
	{
	public:
		Shader(const ShaderDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice), mEntryPoint(desc.EntryPoint),
			mSourceCode(desc.ShaderCode), mStage(desc.Stage),
			mShaderLanguage(desc.Language), mShaderName(desc.ShaderName)
		{}
		virtual ~Shader() override = default;

		const String& GetShaderName() const { return mShaderName; }
		const String& GetEntryPoint() const { return mEntryPoint; }
		const MemoryBuffer& GetSource() const { return mSourceCode; }
		ShaderStage GetStage() const { return mStage; }
		ShaderLanguage GetShaderLanguage() const { return mShaderLanguage; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept override final { return GraphicsDeviceObjectType::Shader; }

	private:
		String mShaderName;
		String mEntryPoint;
		MemoryBuffer mSourceCode;
		ShaderStage mStage;
		ShaderLanguage mShaderLanguage;
	};
}