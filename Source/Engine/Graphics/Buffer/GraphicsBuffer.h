#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Hollow
{
	class GraphicsMemory;
	class ENGINE_API GraphicsBuffer : public DeviceObject
	{
	public:
		GraphicsBuffer(const GraphicsBufferDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
			mUsage(desc.Usage), mBlockSize(desc.BlockSize), mBlockLength(desc.BlockLength),
			mRequestMemory(desc.pRequestMemory)
		{}
		virtual ~GraphicsBuffer() override = default;

		GraphicsBufferUsage GetBufferUsage() const { return mUsage; }
		u32 GetBlockLength() const { return mBlockLength; }
		u32 GetBlockSize() const { return mBlockSize; }
		u32 GetTotalSize() const { return mBlockSize * mBlockLength; }

		WeakPtr<GraphicsMemory> GetMemory() const { return mRequestMemory; }

	private:
		u64 mBlockSize;
		u64 mBlockLength;
		GraphicsBufferUsage mUsage;
		WeakPtr<GraphicsMemory> mRequestMemory;
	};
}