#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <QListWidgetItem>
#include <QString>
#include "vertex.h"
#include "face.h"

class Vertex;   // forward declaration
class Face;     // forward declaration
class HalfEdge : public QListWidgetItem
{
private:
    static int count;           // tracks next available ID to assign to a HalfEdge

    HalfEdge *next_half_edge;       // pointer to next HalfEdge in loop of Face
    HalfEdge *parallel_half_edge;   // pointer to HalfEdge that lies symmetric to this HalfEdge
                                    // and is in loop of Face
    Face *face;                     // pointer to Face that this HalfEdge is on
    Vertex *vertex;                 // pointer to Vertex between this HalfEdge and the next HalfEdge
    int id;                         // unique integer that identifies this Face in menus and while debugging

public:
    // constructors and destructor
    HalfEdge();
    ~HalfEdge();

    // getter methods for private class members
    int get_count();
    HalfEdge *get_next_half_edge();
    HalfEdge *get_parallel_half_edge();
    Face *get_face();
    Vertex *get_vertex();
    int get_id();

    // setter methods for private class members
    void set_next_half_edge(HalfEdge **he);
    void set_parallel_half_edge(HalfEdge **he);
    void set_face(Face **f);
    void set_vertex(Vertex **v);

    static void reset_count();
};

#endif // HALFEDGE_H
