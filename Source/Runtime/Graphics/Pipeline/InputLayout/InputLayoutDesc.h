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
		SemanticName SemanticName;
		TextureFormat InputFormat;
		uint32 SemanticIndex;
		uint32 InputSlotIndex;
		uint32 AlignedByteOffset;
		SlotClass SlotClass;
		uint32 InstanceDataStep;
	};
}
