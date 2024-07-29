#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/ApplicationModule.h>

namespace Hollow
{
	class ImGuiAPI;
	class ImGuiRenderModule : public ApplicationModule
	{
	public:
		ImGuiRenderModule();
		virtual ~ImGuiRenderModule() override = default;

		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
		virtual void Stop() noexcept override;

	protected:
		virtual bool OnInitialize() noexcept override;
		virtual void OnPreUpdate() noexcept override;
		virtual void OnPostUpdate() noexcept override;

	private:
		ImGuiAPI* mImGuiAPI;
	};
}
