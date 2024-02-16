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
		ArrayList<SemanticName> SemanticName;
		ArrayList<TextureFormat> InputFormat;
		ArrayList<uint32> SemanticIndex;
		ArrayList<uint32> InputSlotIndex;
		ArrayList<uint32> AlignedByteOffset;
		ArrayList<SlotClass> SlotClass;
		ArrayList<uint32> InstanceDataStep;
	};
}
