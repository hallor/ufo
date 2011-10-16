#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif
#include "RendererEnums.h"

class vRenderable;

/*  Base class for all rendering properties
    Visualisation data for renderers is to be put in specialised classes derived from this one 
    This class can also be used to synchronize state data between logic and renderig modules   */
class vRenderingPropertiesBase
{
public:
    virtual ~vRenderingPropertiesBase() {};

    /* Used to synchronize data before rendering  */
    virtual void Synchronize(const vRenderable *object) {};
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

    /* Used to synchronize data with rendering properties
       called after given object has been rendered */
    virtual void Synchronize(const vRenderingPropertiesBase *props) {};

    //  Checks whether object is valid for use
    virtual bool IsValid() const;

    //  Retrieves rendering properties
    virtual vRenderingPropertiesBase *GetRenderingProperties() const { return m_RenderingProperties; }

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
