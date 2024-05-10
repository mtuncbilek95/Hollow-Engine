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
		static sharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);

	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mGraphicsAPI(desc.API), mEnabledExtensions(desc.EnabledExtensions), 
		mInstanceName(desc.InstanceName), mApplicationName(desc.ApplicationName) {}
		virtual ~GraphicsInstance() = default;

		// Enumerates all available graphics adapters
		void EnumerateAdapters();
		// Creates a graphics device
		sharedPtr<GraphicsDevice> CreateDevice();
		// Return wanted available graphics adapter
		const GraphicsAdapter& GetAdapter(byte index) const { return mAdapters[index]; }
		// Return all available graphics adapters
		const arrayList<GraphicsAdapter>& GetAdapters() const { return mAdapters; }

		const string& GetInstanceName() const { return mInstanceName; }
		const string& GetApplicationName() const { return mApplicationName; }
		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

	protected:
		virtual void EnumerateAdaptersImpl() = 0;
		virtual sharedPtr<GraphicsDevice> CreateDeviceImpl() = 0;

		void AddAdapter(GraphicsAdapter adapter) { mAdapters.push_back(adapter); }
	private:
		string mInstanceName;
		string mApplicationName;
		GraphicsAPI mGraphicsAPI;
		sharedPtr<GraphicsDevice> mDevice;
		arrayList<GraphicsAdapter> mAdapters;
		arrayList<string> mEnabledExtensions;
	};
}