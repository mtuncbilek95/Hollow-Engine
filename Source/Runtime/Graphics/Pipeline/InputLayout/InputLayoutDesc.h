#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Pipeline/InputLayout/SemanticName.h>
#include <Runtime/Graphics/Pipeline/InputLayout/SlotClass.h>
#include <Runtime/Graphics/Pipeline/InputLayout/PrimitiveMode.h>

#include <Runtime/Graphics/Texture/TextureFormat.h>

namespace Hollow
{
	struct RUNTIME_API InputLayoutDesc
	{
		Array<SemanticName> SemanticName;
		Array<TextureFormat> InputFormat;
		Array<uint32> SemanticIndex;
		Array<uint32> InputSlotIndex;
		Array<uint32> AlignedByteOffset;
		Array<SlotClass> SlotClass;
		Array<uint32> InstanceDataStep;
	};
}
