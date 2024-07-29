#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Core/LogicOperation.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendStateAttachment.h>

namespace Hollow
{
    struct BlendStateDesc
    {
        bool bLogicOperationEnabled;
        LogicOperation LogicOperation;
        DArray<BlendStateAttachment> Attachments;
    };
}