#include <Runtime/Platform/PlatformAPI.h>

#include <EditorPlayer/EditorPlayerApplication.h>
#include <EditorPlayer/EditorPlayerGraphicsModule.h>
#include <EditorPlayer/EditorPlayerWindowModule.h>

using namespace Hollow;

i32 main(i32 argC, char** argV)
{
	PlatformAPI::GetInstanceAPI().InitializeArguments(argC, argV);

	auto mApp = std::make_shared<Hollow::EditorPlayerApplication>();
	mApp->RegisterModule<EditorPlayerWindowModule>();
	mApp->RegisterModule<EditorPlayerGraphicsModule>();

	mApp->Run();

	return 0;
}
