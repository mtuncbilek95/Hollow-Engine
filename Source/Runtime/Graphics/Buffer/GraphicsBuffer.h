#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
	public:
		GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
			mUsage(desc.Usage), mMiscFlags(desc.MiscFlags), mSizeInBytes(desc.SubSizeInBytes * desc.SubCount),
			mSubSizeInBytes(desc.SubSizeInBytes), mSubCount(desc.SubCount), mMemory(desc.pMemory)
		{}
		virtual ~GraphicsBuffer() override = default;

		GraphicsBufferUsage GetUsage() const noexcept { return mUsage; }
		uint8 GetMiscFlags() const noexcept { return mMiscFlags; }
		uint32 GetSizeInBytes() const noexcept { return mSizeInBytes; }
		SharedPtr<GraphicsMemory> GetInitialData() const noexcept { return mMemory; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override { return GraphicsDeviceObjectType::GraphicsBuffer; }

	private:
		GraphicsBufferUsage mUsage;
		uint8 mMiscFlags;
		uint32 mSizeInBytes;
		uint32 mSubSizeInBytes;
		uint32 mSubCount;

		SharedPtr<GraphicsMemory> mMemory;
	};
}
