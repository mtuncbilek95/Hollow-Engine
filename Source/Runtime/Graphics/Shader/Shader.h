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
			mSource(desc.Source), mType(desc.Type), 
			mEntryPoint(desc.EntryPoint), mShaderModel(desc.ShaderModel), mShaderName(desc.ShaderName)
		{}
		virtual ~Shader() override = default;

		const String& GetShaderName() const { return mShaderName; }
		const String& GetSource() const { return mSource; }
		ShaderType GetType() const { return mType; }
		const String& GetEntryPoint() const { return mEntryPoint; }
		const String& GetShaderModel() const { return mShaderModel; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override final { return GraphicsDeviceObjectType::Shader; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		String mShaderName;
		String mSource;
		ShaderType mType;
		String mEntryPoint;
		String mShaderModel;
	};
}
