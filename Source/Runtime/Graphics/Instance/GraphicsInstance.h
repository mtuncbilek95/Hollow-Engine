#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstanceDesc.h>

namespace Hollow
{
	class GraphicsAdapter;
	class RUNTIME_API GraphicsInstance
	{
	public:
		static SharedPtr<GraphicsInstance> Create(const GraphicsInstanceDesc& desc);

	public:
		GraphicsInstance(const GraphicsInstanceDesc& desc) : mGraphicsAPI(desc.API) {}
		virtual ~GraphicsInstance() = default;

		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		SharedPtr<GraphicsAdapter>& GetAdapter(byte index) { return mAdapters[index]; }

	protected:
		void AddAdapter(SharedPtr<GraphicsAdapter> adapter) { mAdapters.push_back(adapter); }

	private:
		Array<SharedPtr<GraphicsAdapter>> mAdapters;
		GraphicsAPI mGraphicsAPI;
	};
}
