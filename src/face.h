#ifndef FACE_H
#define FACE_H

#include <QListWidgetItem>
#include <QString>
#include "halfedge.h"

class HalfEdge;
class Face : public QListWidgetItem
{
private:
    static int count;   // tracks next available ID to assign to a Face

    glm::vec3 color;
    HalfEdge *half_edge;    // pointer to one half-edge that lies on this Face
    int id;                 // unique integer that identifies this Face in menus and while debugging

public:
    // constructors and destructor
    Face();
    Face(glm::vec3 col);
    ~Face();

    // getter methods for private class members
    int get_count();
    glm::vec3 get_color();
    HalfEdge *get_half_edge();
    int get_id();

    // setter methods for private class members
    void set_half_edge(HalfEdge **he);
    void set_col_r(float r);
    void set_col_g(float g);
    void set_col_b(float b);

    static void reset_count();
};

#endif // FACE_H
