#include <Engine/Platform/API/PlatformAPI.h>
#include "TestApp.h"

#include <Engine/Math/ColorConverter.h>

#include <Engine/Serialization/JNumber.h>
#include <Engine/Serialization/JObject.h>
#include <Engine/Serialization/JArray.h>
#include <Engine/Serialization/JString.h>
#include <Engine/Serialization/JBool.h>
#include <Engine/Serialization/API/JSerializer.h>

using namespace Hollow;

int main(i32 argC, char** argV)
{
	PlatformAPI::GetAPI()->InitializeArguments(argC, argV);

	SharedPtr<TestApp> app = MakeShared<TestApp>();
	app->Run();
}