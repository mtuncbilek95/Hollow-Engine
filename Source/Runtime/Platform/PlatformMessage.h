#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	enum RUNTIME_API MessageBoxFlags
	{
		Mb_Ok = 0x00000000L,
		Mb_OkCancel = 0x00000001L,
		Mb_YesNo = 0x00000004L,
		Mb_YesNoCancel = 0x00000003L,
		Mb_RetryCancel = 0x00000005L,

		Mb_UserIcon = 0x00000080L,
		Mb_IconError = 0x00000010L,
		Mb_IconWarning = 0x00000030L,
		Mb_IconQuestion = 0x00000020L,
	};

	class RUNTIME_API PlatformMessage final
	{
	public:
		static void ShowMessage(const String& title, const String& message, const int flags);

	public:
		PlatformMessage() = delete;
		~PlatformMessage() = delete;
	};
}
