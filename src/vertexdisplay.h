#ifndef VERTEXDISPLAY_H
#define VERTEXDISPLAY_H


#include "drawable.h"
#include "vertex.h"

class VertexDisplay : public Drawable
{
protected:
    Vertex *represented_vertex;

public:
    VertexDisplay(OpenGLContext* mp_context);
    virtual GLenum drawMode();
    virtual void create(); /* Creates VBO data to make a visual representation of the currently selected Vertex */

    // getter and setter methods for private class member
    Vertex *get_represented_vertex();
    void set_represented_vertex(Vertex *v); /* Change which Vertex represented_vertex points to */
};

#endif // VERTEXDISPLAY_H
