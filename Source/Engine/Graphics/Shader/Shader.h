#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Shader/ShaderDesc.h>

namespace Hollow
{
	class GraphicsDevice;
	class Shader : public DeviceObject
	{
	public:
		Shader(const ShaderDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
			mShaderName(desc.ShaderName), mEntry(desc.Entry), mStage(desc.Stage), mShaderCode(desc.ShaderCode) {}
		virtual ~Shader() override = default;

		const String& GetShaderName() const { return mShaderName; }
		const String& GetEntry() const { return mEntry; }
		ShaderStage GetStage() const { return mStage; }
		const MemoryBuffer& GetShaderCode() const { return mShaderCode; }

	private:
		String mShaderName;
		String mEntry;
		ShaderStage mStage;
		MemoryBuffer mShaderCode;
	};
}
