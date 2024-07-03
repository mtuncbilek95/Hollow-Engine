#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Object/ManagerAPI.h>
#include <Runtime/Platform/PlatformWindow.h>

namespace Hollow
{
	class RUNTIME_API FreeLookCamera : public ManagerAPI<FreeLookCamera>
	{
	public:
		FreeLookCamera();
		~FreeLookCamera() = default;

		void Update();

		Vec3f GetPosition() const { return mPosition; }
		Vec3f GetOrientation() const { return mOrientation; }
		Vec3f GetUp() const { return mUp; }

		float GetSpeed() const { return mSpeed; }
		float GetSensitivity() const { return mSensitivity; }

		Vec2u GetCameraSize() const { return mCameraSize; }

		Mat4f GetViewMatrix() { return glm::lookAt(mPosition, mPosition + mOrientation, mUp); }
		Mat4f GetProjectionMatrix() { return glm::perspective(glm::radians(70.0f), (float)mCameraSize.x / (float)mCameraSize.y, 0.1f, 100.0f); }

	private:
		Vec3f mPosition;
		Vec3f mOrientation;
		Vec3f mUp;

		float mSpeed;
		float mSensitivity;

		Vec2u mCameraSize;
		bool mFirstMouse;

		GLFWwindow* mWindow;
	};
}
