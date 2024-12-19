#ifndef HALFEDGEDISPLAY_H
#define HALFEDGEDISPLAY_H


#include "drawable.h"
#include "halfedge.h"

class HalfEdgeDisplay : public Drawable
{
protected:
    HalfEdge *represented_half_edge;

public:
    HalfEdgeDisplay(OpenGLContext* mp_context);
    virtual GLenum drawMode();
    virtual void create(); /* Creates VBO data to make a visual representation of the currently selected HalfEdge */

    // getter and setter methods for private class member
    HalfEdge *get_represented_half_edge();
    void set_represented_half_edge(HalfEdge *he); /* Change which HalfEdge represented_half_edge points to */
};

#endif // HALFEDGEDISPLAY_H
