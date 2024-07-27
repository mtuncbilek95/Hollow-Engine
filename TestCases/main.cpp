#include <Engine/Platform/API/PlatformAPI.h>
#include "TestApp.h"

#include <Engine/Math/ColorConverter.h>

using namespace Hollow;

int main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI()->InitializeArguments(argC, argV);

	SharedPtr<TestApp> app = MakeShared<TestApp>();
	app->Run();
}
