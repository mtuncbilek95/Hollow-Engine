#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/BlendState/LogicOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateAttachment.h>


namespace Hollow
{
    struct RUNTIME_API BlendStateDesc
    {
        bool bLogicOperationEnabled;
        LogicOperation LogicOperation;
        Array<BlendStateAttachment> Attachments;
    };
}
