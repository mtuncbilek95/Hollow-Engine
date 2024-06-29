#include "FreeLookCamera.h"

#include <Runtime/Window/WindowManager.h>

namespace Hollow
{
	FreeLookCamera::FreeLookCamera()
	{
		mPosition = Vec3f(0.f, 0.f, -5.f);
		mOrientation = Vec3f(0.f, 0.f, 1.f);
		mUp = Vec3f(0.0f, -1.0f, 0.0f);

		mSpeed = 0.1f;
		mSensitivity = 100.f;

		mCameraSize = WindowManager::GetAPI().GetDefaultWindow()->GetWindowResolution();
		mFirstMouse = true;

		mWindow = WindowManager::GetAPI().GetDefaultWindow()->GetGLFWHandle();
	}

	void FreeLookCamera::Update()
	{
		if(glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			if (mFirstMouse)
			{
				glfwSetCursorPos(mWindow, (mCameraSize.x / 2), (mCameraSize.y / 2));
				mFirstMouse = false;
			}

			if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
				mPosition += mSpeed * mOrientation;
			if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
				mPosition += mSpeed * -Math::normalize(Math::cross(mOrientation, mUp));
			if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
				mPosition += mSpeed * -mOrientation;
			if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
				mPosition += mSpeed * Math::normalize(Math::cross(mOrientation, mUp));
			if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
				mPosition += mSpeed * mUp;
			if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				mPosition += mSpeed * -mUp;
			if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				mSpeed = 0.4f;
			else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
				mSpeed = 0.1f;

			double xpos, ypos;
			glfwGetCursorPos(mWindow, &xpos, &ypos);

			float xoffset = mSensitivity * (float)(ypos - (mCameraSize.y / 2)) / mCameraSize.y;
			float yoffset = mSensitivity * (float)(xpos - (mCameraSize.x / 2)) / mCameraSize.x;

			 Vec3f newOrientation = Math::rotate(mOrientation, Math::radians(xoffset), Math::normalize(Math::cross(mOrientation, mUp)));

			 if (abs(Math::angle(newOrientation, mUp) - Math::radians(90.0f)) <= Math::radians(85.0f))
				 mOrientation = newOrientation;

			 mOrientation = Math::rotate(mOrientation, glm::radians(-yoffset), mUp);

			 glfwSetCursorPos(mWindow, (mCameraSize.x / 2), (mCameraSize.y / 2));
		}
		else if(glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mFirstMouse = true;
		}
	}
}
