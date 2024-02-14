#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/Application.h>

namespace Hollow
{
	class GAME_API GameApplication : public Application
	{
	public:
		GameApplication() = default;
		virtual ~GameApplication() override = default;

	protected:
		void OnInitialize() override;
	};
}
