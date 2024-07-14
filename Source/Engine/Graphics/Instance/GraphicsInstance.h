#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>
#include <Engine/Graphics/Instance/GraphicsInstanceDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsInstance : public IObject
	{
	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mAppName(desc.AppName), 
			mAppVersion(desc.AppVersion), mEngineName(desc.EngineName), 
			mEngineVersion(desc.EngineVersion) {}
		~GraphicsInstance() override = default;

		const String& GetAppName() const { return mAppName; }
		const Vec3f& GetAppVersion() const { return mAppVersion; }
		const String& GetEngineName() const { return mEngineName; }
		const Vec3f& GetEngineVersion() const { return mEngineVersion; }

	private:
		String mAppName;
		Vec3f mAppVersion;

		String mEngineName;
		Vec3f mEngineVersion;
	};
}
