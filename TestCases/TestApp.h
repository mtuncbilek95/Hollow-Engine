#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Application/Application.h>

#include <Engine/Window/WindowModule.h>
#include <Engine/Graphics/API/GraphicsModule.h>
#include <Editor/RenderModules/ImGuiRenderModule.h>
#include <Editor/RenderModules/ImGuiBeginModule.h>
#include <Editor/RenderModules/ImGuiWindowModule.h>
#include <Editor/RenderModules/ImGuiEndModule.h>

namespace Hollow
{
	class TestApp : public Application
	{
	public:
		TestApp() = default;
		virtual ~TestApp() override = default;

	protected:
		virtual void OnInitialize() noexcept override
		{
			// TODO: Add serializer
			// TODO: Add reflection 
			RegisterModule<WindowModule>();
			RegisterModule<GraphicsModule>();
			RegisterModule<ImGuiRenderModule>();
			RegisterModule<ImGuiBeginModule>();
			RegisterModule<ImGuiWindowModule>();
			RegisterModule<ImGuiEndModule>();

			// TODO: Add every singleton structure as modules so we wont have to worry about them in future.
		}
	};
}
