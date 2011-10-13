#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif
#include "RendererEnums.h"

/*  Base class for all rendering properties
    Visualisation data for renderers is to be put in specialised classes derived from this one   */
class vRenderingPropertiesBase
{
public:
    virtual ~vRenderingPropertiesBase() {};

};

class vRenderable
{
public:
    vRenderable();
    virtual ~vRenderable();

    /*  Called right after object creation
        Returns true on initialization success   
        Calling this method on initialized object is legal NOP    */
    virtual bool Initialize();

    /*  Prepares rendering properties
        Use it to synchronize data between logic and visualisation   */
    virtual void PrepareForRendering();

    //  Checks whether object is valid for use
    virtual bool IsValid() const;

    //  Retrieves rendering properties
    virtual const vRenderingPropertiesBase *GetRenderingProperties() const { return m_RenderingProperties; }

    //  Retrieves type of renderable object
    ERenderableType::TYPE GetRenderableType() const { return m_RenderableType; }

protected:

    /*  Do not call this method directly, it will be called by Initialize()
        Every specialized vRenderable type has to overload this method to correctly create their rendering properties
        Returns true when creation of properties has succedeed   */
    virtual bool CreateRenderingProperties();

    /*  Do not call this method directly, it will be called by ~vRendearble()
        Deletes rendering properties    */
    virtual void DeleteRenderingProperties();

    // Type of renderable object
    ERenderableType::TYPE m_RenderableType;
    // Properties for renderers
    vRenderingPropertiesBase *m_RenderingProperties;
};
