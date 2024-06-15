#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/BasicCamera/BaseCamera.h>

namespace Hollow
{
	class RUNTIME_API FreeLookCamera : public BaseCamera
	{
	public:
		FreeLookCamera();
		~FreeLookCamera() = default;

		virtual void Update(float deltaTime) override;
		virtual void UpdateInput(float deltaTime) override;
	};
}