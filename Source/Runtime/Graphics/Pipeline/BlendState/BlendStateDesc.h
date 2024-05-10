#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Common/LogicOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateAttachment.h>

namespace Hollow
{
    struct RUNTIME_API BlendStateDesc
    {
        bool bLogicOperationEnabled;
        LogicOperation LogicOperation;
        arrayList<BlendStateAttachment> Attachments;
    };
}