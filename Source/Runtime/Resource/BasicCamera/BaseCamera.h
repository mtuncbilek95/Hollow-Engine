#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Window/WindowManager.h>

namespace Hollow
{
	class RUNTIME_API BaseCamera
	{
	public:
		BaseCamera() = default;
		~BaseCamera() = default;

		void GetHandle()
		{
			mViewportSize = WindowManager::GetInstanceAPI().GetDefaultWindow()->GetWindowResolution();
			mWindowHandle = WindowManager::GetInstanceAPI().GetDefaultWindow()->GetGLFWHandle();
		}

		virtual void Update(f32 deltaTime) = 0;
		virtual void UpdateInput(f32 deltaTime) = 0;

		MatrixSIMD GetViewMatrix() const { return mViewMatrix; }
		MatrixSIMD GetProjectionMatrix() const { return mProjectionMatrix; }
		Vector3f GetPosition() const { return mPosition; }

	protected:
		Vector3f mPosition = { 0.0f, 0.0f, 0.0f };
		Vector3f mOrientation = { 0.0f, 0.0f, -1.0f };
		Vector3f mUp = { 0.0f, 1.0f, 0.0f };

		bool mIsDirty = true;
		Vector2u mViewportSize = { 1300, 1300 };
		f32 mAspectRatio = 0.0f;
		f32 mFieldOfView = 0.0f;
		f32 mNearPlane = 0.0f;
		f32 mFarPlane = 0.0f;

		f32 mSpeed = 0.1f;
		f32 mSensitivity = 100.f;

		MatrixSIMD mProjectionMatrix = XMMatrixIdentity();
		MatrixSIMD mViewMatrix = XMMatrixIdentity();

		GLFWwindow* mWindowHandle;

		XMFLOAT3 RotateVector(XMFLOAT3 vector, f32 angle, XMFLOAT3 axis)
		{
			XMVECTOR xmVector = XMLoadFloat3(&vector);
			XMVECTOR xmAxis = XMLoadFloat3(&axis);
			XMMATRIX rotationMatrix = XMMatrixRotationAxis(xmAxis, XMConvertToRadians(angle));
			xmVector = XMVector3TransformCoord(xmVector, rotationMatrix);
			XMStoreFloat3(&vector, xmVector);
			return vector;
		}
	};
}
