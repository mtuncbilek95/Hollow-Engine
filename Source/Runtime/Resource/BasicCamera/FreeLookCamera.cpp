#include "FreeLookCamera.h"

namespace Hollow
{
	FreeLookCamera::FreeLookCamera() : BaseCamera()
	{
		mAspectRatio = (f32)mViewportSize.x / (f32)mViewportSize.y;
		mFieldOfView = 74.0f;
		mNearPlane = 0.1f;
		mFarPlane = 10000.0f;
		mSpeed = 1.f;

		mSensitivity = 100.0f;
		mPosition = { 0.0f, 0.0f, -5.0f };
		mOrientation = { 0.0f, 0.0f, 1.0f };
		mUp = { 0.0f, 1.0f, 0.0f };
		mViewMatrix = XMMatrixIdentity();
		mProjectionMatrix = XMMatrixIdentity();
	}

	void FreeLookCamera::Update(f32 deltaTime)
	{
		mAspectRatio = (f32)mViewportSize.x / (f32)mViewportSize.y;
		mViewportSize = WindowManager::GetInstanceAPI().GetDefaultWindow()->GetWindowResolution();
		mViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&mOrientation) + XMLoadFloat3(&mPosition), XMLoadFloat3(&mUp));
		mProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
	}

	void FreeLookCamera::UpdateInput(f32 deltaTime)
	{
		if (glfwGetMouseButton(mWindowHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if (glfwGetKey(mWindowHandle, GLFW_KEY_W) == GLFW_PRESS)
				mPosition = { mPosition.x + mOrientation.x * mSpeed, mPosition.y + mOrientation.y * mSpeed, mPosition.z + mOrientation.z * mSpeed };
			if (glfwGetKey(mWindowHandle, GLFW_KEY_S) == GLFW_PRESS)
				mPosition = { mPosition.x - mOrientation.x * mSpeed, mPosition.y - mOrientation.y * mSpeed, mPosition.z - mOrientation.z * mSpeed };
			if (glfwGetKey(mWindowHandle, GLFW_KEY_A) == GLFW_PRESS)
			{
				XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp)));
				XMFLOAT3 rightFloat;
				XMStoreFloat3(&rightFloat, right);
				mPosition = { mPosition.x + rightFloat.x * mSpeed, mPosition.y + rightFloat.y * mSpeed, mPosition.z + rightFloat.z * mSpeed };
			}
			if (glfwGetKey(mWindowHandle, GLFW_KEY_D) == GLFW_PRESS)
			{
				XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp)));
				XMFLOAT3 rightFloat;
				XMStoreFloat3(&rightFloat, right);
				mPosition = { mPosition.x - rightFloat.x * mSpeed, mPosition.y - rightFloat.y * mSpeed, mPosition.z - rightFloat.z * mSpeed };
			}
			if (glfwGetKey(mWindowHandle, GLFW_KEY_SPACE) == GLFW_PRESS)
				mPosition = { mPosition.x - mUp.x * mSpeed, mPosition.y - mUp.y * mSpeed, mPosition.z - mUp.z * mSpeed };
			if (glfwGetKey(mWindowHandle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				mPosition = { mPosition.x + mUp.x * mSpeed, mPosition.y + mUp.y * mSpeed, mPosition.z + mUp.z * mSpeed };

			if (glfwGetKey(mWindowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				mSpeed = 2.0f;
			else if (glfwGetKey(mWindowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
				mSpeed = 1.0f;

			glfwSetInputMode(mWindowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (mIsDirty)
			{
				glfwSetCursorPos(mWindowHandle, mViewportSize.x / 2.0f, mViewportSize.y / 2.0f);
				mIsDirty = false;
			}

			double mouseX, mouseY;

			glfwGetCursorPos(mWindowHandle, &mouseX, &mouseY);

			float rotX = mSensitivity * (float)(mouseY - (mViewportSize.y / 2.0f)) / mViewportSize.y;
			float rotY = mSensitivity * (float)(-mouseX + (mViewportSize.x / 2.0f)) / mViewportSize.x;

			XMFLOAT3 tempAxis;
			XMStoreFloat3(&tempAxis, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&mOrientation), XMLoadFloat3(&mUp))));

			XMFLOAT3 tempOrientation = RotateVector(mOrientation, rotX, tempAxis);

			if (abs(XMVectorGetX(XMVector3AngleBetweenNormals(XMLoadFloat3(&tempOrientation), XMLoadFloat3(&mUp))) - XMConvertToRadians(90.0f)) <= XMConvertToRadians(85.0f))
				mOrientation = tempOrientation;

			mOrientation = RotateVector(mOrientation, -rotY, mUp);

			glfwSetCursorPos(mWindowHandle, (mViewportSize.x / 2.0f), (mViewportSize.y / 2.0f));
		}
		else if (glfwGetMouseButton(mWindowHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			glfwSetInputMode(mWindowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mIsDirty = true;
		}
	}
}