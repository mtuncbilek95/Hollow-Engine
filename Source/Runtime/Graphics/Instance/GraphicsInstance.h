#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Runtime/Graphics/Instance/GraphicsAdapter.h>

namespace Hollow
{
	class GraphicsDevice;
	struct GraphicsDeviceDesc;

	class RUNTIME_API GraphicsInstance
	{
	public:
		static SharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);

	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mGraphicsAPI(desc.API), mEnabledExtensions(desc.EnabledExtensions), 
		mInstanceName(desc.InstanceName), mApplicationName(desc.ApplicationName) {}
		virtual ~GraphicsInstance() = default;

		// Enumerates all available graphics adapters
		void EnumerateAdapters();
		// Creates a graphics device
		SharedPtr<GraphicsDevice> CreateDevice();
		// Return wanted available graphics adapter
		const GraphicsAdapter& GetAdapter(byte index) const { return mAdapters[index]; }
		// Return all available graphics adapters
		const Array<GraphicsAdapter>& GetAdapters() const { return mAdapters; }

		const String& GetInstanceName() const { return mInstanceName; }
		const String& GetApplicationName() const { return mApplicationName; }
		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

	protected:
		virtual void EnumerateAdaptersImpl() = 0;
		virtual SharedPtr<GraphicsDevice> CreateDeviceImpl() = 0;

		void AddAdapter(GraphicsAdapter adapter) { mAdapters.push_back(adapter); }
	private:
		String mInstanceName;
		String mApplicationName;
		GraphicsAPI mGraphicsAPI;
		SharedPtr<GraphicsDevice> mDevice;
		Array<GraphicsAdapter> mAdapters;
		Array<String> mEnabledExtensions;
	};
}