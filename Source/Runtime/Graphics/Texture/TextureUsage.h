#pragma once

namespace Hollow
{
    enum class RUNTIME_API TextureUsage
    {
        Unknown,
        ShaderResource,
        RenderTarget,
        DepthStencil,
        UnorderedAccess
    };
}
