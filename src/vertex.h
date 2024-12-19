#ifndef VERTEX_H
#define VERTEX_H

#include <la.h>
#include <QListWidgetItem>
#include <QString>
#include "halfedge.h"
#include "joint.h"

class HalfEdge;     // forward declaration
class Vertex : public QListWidgetItem
{
    const int MAX_NUM_JOINT_INFLUENCES = 2;

private:
    static int count;   // tracks next available ID to assign to a Vertex

    glm::vec3 position;
    HalfEdge *half_edge;    // pointer to one half-edge that points to this Vertex
    int id;                 // unique integer that identifies this Vertex in menus and while debugging
    std::map<int, float> joints;  // maps IDs of the Joints that influence this Vertex's transformation
                                  // to the weight of the Joint's influence

public:
    // constructors and destructor
    Vertex();
    Vertex(glm::vec3 pos);
    ~Vertex();

    // getter methods for private class members
    int get_count();
    glm::vec3 get_position();
    HalfEdge *get_half_edge();
    int get_id();
    std::map<int, float> get_joints();

    // setter methods for private class members
    void set_half_edge(HalfEdge **he);
    void set_pos_x(float x);
    void set_pos_y(float y);
    void set_pos_z(float z);
    void get_influencing_joints(std::vector<Joint*> &influencing_joints, Joint *curr_joint);
    void populate_influencing_joints(Joint *curr_joint);

    static void reset_count();
};

#endif // VERTEX_H
