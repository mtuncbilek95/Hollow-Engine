#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Runtime/Graphics/Instance/GraphicsAdapter.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc;
	class RUNTIME_API GraphicsDevice;

	class RUNTIME_API GraphicsInstance : public Object
	{
	public:
		static SharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);

	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc);
		virtual ~GraphicsInstance() = default;

		void EnumerateAdapters();
		
		SharedPtr<GraphicsDevice> CreateGraphicsDevice(const GraphicsDeviceDesc& desc);

		const SharedPtr<GraphicsAdapter> GetAdapter(byte index) const { return mAdapters[index]; }
		const ArrayList<SharedPtr<GraphicsAdapter>>& GetAdapters() const { return mAdapters; }

		const String& GetInstanceName() const { return mInstanceName; }
		const String& GetApplicationName() const { return mApplicationName; }
		const GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() override = 0;

	protected:
		virtual void EnumerateAdaptersImpl() = 0;
		virtual SharedPtr<GraphicsDevice> CreateDeviceImpl(const GraphicsDeviceDesc& desc) = 0;

		void AddAdapter(SharedPtr<GraphicsAdapter> adapter) { mAdapters.push_back(adapter); }

	private:
		String mInstanceName;
		String mApplicationName;

		GraphicsAPI mGraphicsAPI;

		SharedPtr<GraphicsDevice> mOwnedDevice;
		ArrayList<SharedPtr<GraphicsAdapter>> mAdapters;
		ArrayList<String> mInstanceExtensions;
	};
}
