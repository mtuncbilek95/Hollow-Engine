#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Object/ManagerAPI.h>

namespace Hollow
{
	class RUNTIME_API FreeLookCamera : public Object, ManagerAPI<FreeLookCamera>
	{
	public:
		FreeLookCamera();
		~FreeLookCamera();

		void Update();

		Vec3f GetPosition() const { return mPosition; }
		Vec3f GetOrientation() const { return mOrientation; }
		Vec3f GetUp() const { return mUp; }

		float GetSpeed() const { return mSpeed; }
		float GetSensitivity() const { return mSensitivity; }

		Vec2u GetCameraSize() const { return mCameraSize; }

	private:
		Vec3f mPosition;
		Vec3f mOrientation;
		Vec3f mUp;

		float mSpeed;
		float mSensitivity;

		Vec2u mCameraSize;

		bool mFirstMouse;
	};
}
