#pragma once

namespace EEngineHandler
{
    enum TYPE
    {
        HandlerOnLogicUpdate = 0, // Called each logic update
        HandlerOnRenderFrame, // Called each rendering frame, after logic update but before actual rendering is done
        _COUNT,

        _First = HandlerOnLogicUpdate,
        _Last = HandlerOnRenderFrame
    };

    bool IsValid(TYPE t);
}