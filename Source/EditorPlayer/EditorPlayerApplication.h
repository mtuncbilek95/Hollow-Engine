#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Application/Application.h>

namespace Hollow
{
	class EDITORPLAYER_API EditorPlayerApplication : public Application
	{
	public:
		EditorPlayerApplication();
		~EditorPlayerApplication() override = default;

		virtual void OnShutdown() noexcept override;

	protected:
		virtual void OnInitialize() noexcept override;
	};
}
