#pragma once

class vRenderable;

//   Base class for all renderers
class iRendererBase
{
public:
    //  Force virtual destructors
    virtual ~iRendererBase();

    //  Returns true when object is ready for use
    virtual bool IsValid() const = 0;

    /*  Called every logical frame
        dt is time from last logical frame, it doesn't have to be real time   */
    virtual void OnFrame(float dt);

    //  Called to render object using its current properties
    virtual void Render(vRenderable *object) = 0;
};