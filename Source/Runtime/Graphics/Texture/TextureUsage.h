#pragma once

namespace Hollow
{
    enum class RUNTIME_API TextureUsage
    {
        Unknown,
        ShaderResource,
        ColorAttachment,
        DepthStencil,
        Storage,
        TransferSrc,
        TransferDst,
        TransientAttachment,
        InputAttachment
    };
}