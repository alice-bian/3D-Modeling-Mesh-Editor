#ifndef JOINT_H
#define JOINT_H

#include "smartpointerhelp.h"
#include <la.h>
#include <QTreeWidgetItem>
#include <glm/gtx/quaternion.hpp>


class Joint : public QTreeWidgetItem
{

    friend class MyGL;
    friend class JointDisplay;

protected:
    static int count;   // tracks next available ID to assign to a Joint

    int id;                                 // unique integer that identifies this Joint while debugging
    QString name;                           // name of Joint
    Joint *parent;                          // parent of this Joint
    std::vector<uPtr<Joint>> children;      // set of Joint's children
    glm::vec3 pos;                          // position of Joint relative to parent Joint
    glm::quat rotation;                     // quaternion that represents Joint's current
                                            // orientation through w + xi + yj +zk
    glm::mat4 bind_matrix;                  // inverse of Joint's compound transformation matrix
                                            // at the time a mesh is bound to the Joint's skeleton

public:
    /* Constructors and destructor */
    Joint(QString name, glm::vec3 pos, glm::quat rotation);
    ~Joint();

    /* Methods */
    Joint& add_child(uPtr<Joint> child);    // Adds a new child to children vector.
    glm::mat4 get_local_transformation();   // Returns a 4x4 matrix representing concatenation
                                            // of a Joint's position and rotation.
    glm::mat4 get_overall_transformation(); // Returns a 4x4 matrix representing concatenation
                                            // of Joint's local transformation with transformations
                                            // of its chain of parent joints.
    glm::mat4 set_bind_matrix();
    Joint& operator=(const Joint &j2);

    // getter and setter methods
    int get_id();
    glm::vec3 get_pos();
    std::vector<Joint*> get_children();
    void set_parent(Joint *j);
    void set_pos_x(float x);
    void set_pos_y(float y);
    void set_pos_z(float z);
    glm::quat get_rot();
    void set_rot(glm::quat q);
};

#endif // JOINT_H
