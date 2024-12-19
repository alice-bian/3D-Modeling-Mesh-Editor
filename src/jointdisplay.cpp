#include "jointdisplay.h"
#include <iostream>

JointDisplay::JointDisplay(OpenGLContext* mp_context) : Drawable(mp_context),
                                                        represented_joint(nullptr)
{}

GLenum JointDisplay::drawMode() {
    return GL_LINES;
}

// FOR DEBUGGING
void JointDisplay::print_joint_tree_pos(Joint *curr_joint) {
    std::cout << glm::to_string(curr_joint->get_pos()) << std::endl;
    for (const uPtr<Joint>& child : curr_joint->children) {
        Joint *child_ptr = child.get();
        print_joint_tree_pos(child_ptr);
    }
}

int JointDisplay::create_helper(Joint *curr_joint, glm::vec3 &curr_pos,
                                 std::vector<glm::vec4> &pos,
                                 std::vector<glm::vec4> &col,
                                 std::vector<GLuint> &idx, int starting_idx) {
    // fill pos
    for (float i = 0; i < CIRCLE_MAX_DEG; i+=(CIRCLE_MAX_DEG / CIRCLE_NUM_SIDES)) {
        pos.push_back((curr_joint->get_overall_transformation())
                       * glm::vec4(CIRCLE_RADIUS * glm::vec3(glm::cos(glm::radians(i)),
                                                             glm::sin(glm::radians(i)),
                                                             0.f),
                                   1.f));
    }
    for (float i = 0; i < CIRCLE_MAX_DEG; i+=(CIRCLE_MAX_DEG / CIRCLE_NUM_SIDES)) {
        pos.push_back((curr_joint->get_overall_transformation())
                       * glm::vec4(CIRCLE_RADIUS * glm::vec3(0.f,
                                                             glm::cos(glm::radians(i)),
                                                             glm::sin(glm::radians(i))),
                                   1.f));
    }
    for (float i = 0; i < CIRCLE_MAX_DEG; i+=(CIRCLE_MAX_DEG / CIRCLE_NUM_SIDES)) {
        pos.push_back((curr_joint->get_overall_transformation())
                       * glm::vec4(CIRCLE_RADIUS * glm::vec3(glm::cos(glm::radians(i)),
                                                             0.f,
                                                             glm::sin(glm::radians(i))),
                                   1.f));
    }

    // fill col
    if (curr_joint == selected_joint) {
        for (int i = 0; i < CIRCLE_NUM_SIDES * 3; i++) {    // highlight selected joint
            col.push_back(glm::vec4(1, 1, 1, 0));   // all white circles
        }
    } else {
        for (int i = 0; i < CIRCLE_NUM_SIDES; i++) {
            col.push_back(glm::vec4(0, 0, 1, 0));   // xy-plane (blue circle)
        }
        for (int i = 0; i < CIRCLE_NUM_SIDES; i++) {
            col.push_back(glm::vec4(1, 0, 0, 0));   // yz-plane (red circle)
        }
        for (int i = 0; i < CIRCLE_NUM_SIDES; i++) {
            col.push_back(glm::vec4(0, 1, 0, 0));   // xz-plane (green circle)
        }
    }

    // fill idx
    for (int i = 0; i < 3; i++) {
        for (int j = starting_idx; j < starting_idx + CIRCLE_NUM_SIDES; j++) {
            idx.push_back(j);
            if (j == starting_idx + CIRCLE_NUM_SIDES - 1) {
                idx.push_back(starting_idx);
            } else {
                idx.push_back(j + 1);
            }
        }
        starting_idx += CIRCLE_NUM_SIDES;
    }

    // draw lines connecting parent Joint to children Joints
    std::vector<Joint*> children = curr_joint->get_children();
    for (Joint* child : children) {
        curr_joint = child;
        curr_pos += curr_joint->pos;

        glm::vec4 endpoint1 = curr_joint->parent->get_overall_transformation()[3];  // parent end
        glm::vec4 endpoint2 = curr_joint->get_overall_transformation()[3];     // child end
        pos.push_back(endpoint1);
        pos.push_back(endpoint2);
        col.push_back(glm::vec4(1.f, 0.f, 1.f, 0.f));       // magenta
        col.push_back(glm::vec4(1.f, 1.f, 0.f, 0.f));       // yellow
        idx.push_back(starting_idx);
        starting_idx++;
        idx.push_back(starting_idx);
        starting_idx++;

        // recurse on children
        starting_idx = create_helper(curr_joint, curr_pos, pos, col, idx, starting_idx);
    }

    return starting_idx;
}

void JointDisplay::create() {
    // only draw if root exists!
    if (represented_joint == nullptr) {
        return;
    }

    // iterate through current Joint and all its children until there are no more children left
    Joint *curr_joint = represented_joint;
    glm::vec3 curr_pos = curr_joint->pos;

    // set up all VBOs (initially empty)
    std::vector<glm::vec4> pos = std::vector<glm::vec4>();
    std::vector<glm::vec4> col = std::vector<glm::vec4>();
    std::vector<GLuint> idx = std::vector<GLuint>();
    int starting_idx = 0;

    create_helper(curr_joint, curr_pos, pos, col, idx, starting_idx);

    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
}

Joint *JointDisplay::get_represented_joint() {
    return represented_joint;
}

void JointDisplay::set_represented_joint(Joint *j) {
    represented_joint = j;
}

Joint *JointDisplay::get_selected_joint() {
    return selected_joint;
}

void JointDisplay::set_selected_joint(Joint *j) {
    selected_joint = j;
}
