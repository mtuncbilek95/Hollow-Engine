#include "TestApp.h"

using namespace Hollow;

int main()
{
	SharedPtr<TestApp> app = MakeShared<TestApp>();
	app->Run();
}
