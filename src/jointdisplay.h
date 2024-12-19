#ifndef JOINTDISPLAY_H
#define JOINTDISPLAY_H

#include "drawable.h"
#include "joint.h"


class JointDisplay : public Drawable
{
    const float CIRCLE_NUM_SIDES = 12;
    const float CIRCLE_MAX_DEG = 360;
    const float CIRCLE_RADIUS = 0.5f;

protected:
    Joint *represented_joint;   // will always store the root Joint
    Joint *selected_joint;

public:
    JointDisplay(OpenGLContext* mp_context);
    virtual GLenum drawMode();

    /* Recursively draws curr Joint and all its child Joints. Returns next starting_idx. */
    int create_helper(Joint *curr_joint, glm::vec3 &curr_pos,
                       std::vector<glm::vec4> &pos,
                       std::vector<glm::vec4> &col,
                       std::vector<GLuint> &idx, int starting_idx);
    virtual void create(); /* Creates VBO data to make a visual representation of the currently selected Joint */

    // getter and setter methods for private class member
    Joint *get_represented_joint();
    void set_represented_joint(Joint *j); /* Change which Joint represented_joint points to */
    Joint *get_selected_joint();
    void set_selected_joint(Joint *j); /* Change which Joint selected_joint points to */

    // FOR DEBUGGING
    void print_joint_tree_pos(Joint *curr_joint);
};

#endif // JOINTDISPLAY_H
