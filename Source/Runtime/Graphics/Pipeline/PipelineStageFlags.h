#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
    enum class RUNTIME_API PipelineStageFlags : u16
    {
        TopOfPipe = 1,
        VertexInput = 2,
        VertexShader = 4,
        TessellationControlShader = 8,
        TessellationEvaluationShader = 16,
        GeometryShader = 32,
        FragmentShader = 64,
        EarlyFragmentTests = 128,
        LateFragmentTests = 256,
        ColorAttachmentOutput = 512,
        ComputeShader = 1024,
        Transfer = 2048,
        BottomOfPipe = 4096,
		Host = 8192,
		AllGraphics = 16384,
		AllCommands = 32768
    };

    GENERATE_ENUM_FLAG(PipelineStageFlags, u16);
}