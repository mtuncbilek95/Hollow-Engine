#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class ImGuiWindowAPI;
	class ImGuiWindowModule : public ApplicationModule
	{
	public:
		ImGuiWindowModule();
		virtual ~ImGuiWindowModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual bool OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		ImGuiWindowAPI* mImGuiAPI;
	};
}
