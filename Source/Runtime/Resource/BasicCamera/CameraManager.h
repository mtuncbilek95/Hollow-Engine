#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Resource/BasicCamera/BaseCamera.h>
#include <Runtime/Resource/BasicCamera/FreeLookCamera.h>
#include <Runtime/Object/ManagerAPI.h>

namespace Hollow
{
	enum class RUNTIME_API CameraType
	{
		None,
		FreeLook,
		FirstPerson,
		ThirdPerson,
		Orthographic
	};

	class RUNTIME_API CameraManager : public ManagerAPI<CameraManager>
	{
	public:
		CameraManager() = default;
		~CameraManager() = default;

		SharedPtr<BaseCamera> CreateCamera(CameraType type)
		{
			if (mCamera == nullptr)
			{
				switch (type)
				{
				case CameraType::FreeLook:
					mCamera = std::make_shared<FreeLookCamera>();
					break;
				default:
					break;
				}
			}
			return mCamera;
		}

		SharedPtr<BaseCamera>& GetCamera()
		{
			if (mCamera == nullptr)
				mCamera = std::make_shared<FreeLookCamera>();

			return mCamera;
		}

		private:
			SharedPtr<BaseCamera> mCamera;
		};
	}