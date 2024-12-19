#ifndef MESH_H
#define MESH_H

#include "drawable.h"
#include <la.h>

#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include "smartpointerhelp.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class Mesh : public Drawable
{
private:
    std::vector<uPtr<Vertex>> vertices;
    std::vector<uPtr<HalfEdge>> half_edges;
    std::vector<uPtr<Face>> faces;
    bool is_skinned;

public:
    Mesh(OpenGLContext* mp_context);
    void create_cube_mesh();  /* Builds a cube-shaped mesh using HalfEdges */

    // transforms private class vector of unique pointers to vector of raw pointers
    std::vector<Vertex*> get_vertex_pointers();
    std::vector<HalfEdge*> get_half_edge_pointers();
    std::vector<Face*> get_face_pointers();

    // adds new Vertex, HalfEdge, and Face to QListWidget upon splitting and/or triangulation
    Vertex& add_vertex(uPtr<Vertex> &vert);
    HalfEdge& add_half_edge(uPtr<HalfEdge> &he);
    Face& add_face(uPtr<Face> &f);

    // helper method to populate idx VBO, call on in create()
    std::vector<GLuint> triangulate(int starting_vertex_num, int num_sides);
    virtual void create();
    void clear();

    // getter and setter functions
    bool get_is_skinned();
    void set_is_skinned(bool skinned);
};

#endif // MESH_H
