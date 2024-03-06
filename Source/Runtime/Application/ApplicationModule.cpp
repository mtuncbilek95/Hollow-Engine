#include "ApplicationModule.h"

#include <Runtime/Application/Application.h>

namespace Hollow
{
	void ApplicationModule::SendQuitReason(const String& reason)
	{
		mOwnerApplication->QuitReason(reason);
	}
}
