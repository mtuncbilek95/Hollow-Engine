#pragma once

namespace Hollow
{
    enum class RUNTIME_API TextureUsage
    {
        Unknown,
        Default,
        Dynamic,
        Read_Write,
        Scratch,
        Field,
        Shader_Input,
        Render_Target_Output,
        Back_Buffer,
        Shared,
        Read_Only,
        Discard,
        Unordered,
    };
}
