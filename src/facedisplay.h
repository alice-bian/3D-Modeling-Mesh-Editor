#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H


#include "drawable.h"
#include "face.h"

class FaceDisplay : public Drawable
{
protected:
    Face *represented_face;

public:
    FaceDisplay(OpenGLContext* mp_context);
    virtual GLenum drawMode();
    virtual void create(); /* Creates VBO data to make a visual representation of the currently selected Face */

    // getter and setter methods for private class member
    Face *get_represented_face();
    void set_represented_face(Face *f); /* Change which Face represented_face points to */
};

#endif // FACEDISPLAY_H
