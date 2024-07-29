#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class GraphicsAPI;
	class GraphicsModule : public ApplicationModule
	{
	public:
		GraphicsModule();
		virtual ~GraphicsModule() override;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual bool OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		GraphicsAPI* mGraphicsAPI;
	};
}
