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

		String GetProductName() const { return mProductName; }
		uint32 GetVRam() const { return mVRam; }

	protected:
		virtual SharedPtr<GraphicsDevice> CreateDeviceCore(const GraphicsDeviceDesc& desc) = 0;

	private:
		SharedPtr<GraphicsDevice> mDevice;
		String mProductName;
		uint32 mVRam;
		GraphicsInstance* mInstance;
	};
}
