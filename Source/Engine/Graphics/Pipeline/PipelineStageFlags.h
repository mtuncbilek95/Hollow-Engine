#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
    enum class ENGINE_API PipelineStageFlags : u32
    {
        TopOfPipe = 1 << 0,
        VertexInput = 1 << 1,
        VertexShader = 1 << 2,
        TessellationControlShader = 1 << 3,
        TessellationEvaluationShader = 1 << 4,
        GeometryShader = 1 << 5,
        FragmentShader = 1 << 6,
        EarlyFragmentTests = 1 << 7,
        LateFragmentTests = 1 << 8,
        ColorAttachmentOutput = 1 << 9,
        ComputeShader = 1 << 10,
        Transfer = 1 << 11,
        BottomOfPipe = 1 << 12,
		Host = 1 << 13,
		AllGraphics = 1 << 14,
		AllCommands = 1 << 15
    };

    GENERATE_ENUM_FLAG(PipelineStageFlags, u32);
}