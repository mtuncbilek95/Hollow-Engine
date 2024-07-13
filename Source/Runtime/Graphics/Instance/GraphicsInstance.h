#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/IObject.h >
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>
#include <Runtime/Graphics/Instance/GraphicsAdapter.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc;
	class RUNTIME_API GraphicsDevice;

	/**
	 * @class GraphicsInstance
	 * @brief GraphicsInstance is the main entry point for the graphics API.
	 */
	class RUNTIME_API GraphicsInstance : public IObject
	{
	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc);
		virtual ~GraphicsInstance() = default;

		void EnumerateAdapters();
		
		SharedPtr<GraphicsDevice> CreateGraphicsDevice(const GraphicsDeviceDesc& desc);

		const SharedPtr<GraphicsAdapter> GetAdapter(byte index) const { return mAdapters[index]; }
		const DArray<SharedPtr<GraphicsAdapter>>& GetAdapters() const { return mAdapters; }

		const String& GetInstanceName() const { return mInstanceName; }
		const String& GetApplicationName() const { return mApplicationName; }
		const GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

	protected:
		virtual void EnumerateAdaptersImpl() = 0;

		void AddAdapter(SharedPtr<GraphicsAdapter> adapter) { mAdapters.push_back(adapter); }

	private:
		String mInstanceName;
		String mApplicationName;

		GraphicsAPI mGraphicsAPI;

		SharedPtr<GraphicsDevice> mOwnedDevice;
		DArray<SharedPtr<GraphicsAdapter>> mAdapters;
	};
}
