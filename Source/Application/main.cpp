#include <Application/GameApplication.h>
#include <Application/GameWindowModule.h>
#include <Application/GameGraphicsModule.h>

using namespace Hollow;

int main()
{
	GameApplication app;

	app.RegisterModule<GameWindowModule>();
	app.RegisterModule<GameGraphicsModule>();

	app.Run();

	return 0;
}
