#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorLayout : public GraphicsDeviceObject
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, const SharedPtr<GraphicsDevice> device) : GraphicsDeviceObject(device), mEntries(desc.Entries) {}
		virtual ~DescriptorLayout() override = default;

		const ArrayList<DescriptorLayoutEntry>& GetEntries() const noexcept { return mEntries; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorLayout; }
		virtual void OnShutdown() noexcept override = 0;

	private:
		ArrayList<DescriptorLayoutEntry> mEntries;
	};
}