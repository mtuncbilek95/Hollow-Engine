#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/API.h>

namespace Hollow
{
	class RUNTIME_API PlatformAPI : public API<PlatformAPI>
	{
	public:
		PlatformAPI() = default;
		virtual ~PlatformAPI() override = default;

		void InitializeArguments(i32 argc, char** argv)
		{
			String enginePath = argv[0];
			String searchString = "Hollow-Engine";

			size_t pos = enginePath.find(searchString);
			if (pos != std::string::npos) {
				size_t lastSlashPos = enginePath.rfind("/\\", pos);
				if (lastSlashPos != std::string::npos)
					mEngineSourcePath = enginePath.substr(0, lastSlashPos + 1);
				else
					mEngineSourcePath = enginePath.substr(0, pos + searchString.length()) + "\\";
			}

			if (argc > 1)
				mProjectPath = argv[1];
		}

		const String& GetEngineSourcePath() const { return mEngineSourcePath; }
		const String& GetProjectPath() const { return mProjectPath; }

	private:
		String mEngineSourcePath;
		String mProjectPath;
	};
}
