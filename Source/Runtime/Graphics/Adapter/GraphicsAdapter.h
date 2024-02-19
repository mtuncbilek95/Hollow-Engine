#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapterDesc.h>

namespace Hollow
{
	class GraphicsDevice;
	struct GraphicsDeviceDesc;

	class RUNTIME_API GraphicsAdapter
	{
	public:
		GraphicsAdapter(const GraphicsAdapterDesc& desc);
		virtual ~GraphicsAdapter() = default;

		void CreateDevice(const GraphicsDeviceDesc& desc);

		SharedPtr<GraphicsDevice> GetDevice() { return mDevice; }
		GraphicsInstance* GetInstance() { return mInstance; }

		Vector3u GetImageDimensions() const { return mImageDimensions; }
		String GetProductName() const { return mProductName; }
		uint32 GetVRam() const { return mVRam; }

		virtual void OnShutdown() = 0;

	protected:
		virtual SharedPtr<GraphicsDevice> CreateDeviceCore(const GraphicsDeviceDesc& desc) = 0;

	private:
		SharedPtr<GraphicsDevice> mDevice;
		Vector3u mImageDimensions;
		String mProductName;
		uint32 mVRam;
		GraphicsInstance* mInstance;
	};
}
