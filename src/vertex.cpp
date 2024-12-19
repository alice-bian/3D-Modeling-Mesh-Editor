#include "vertex.h"
#include <iostream>

// initialize static variable
int Vertex::count = 0;

Vertex::Vertex() : position(glm::vec3()), half_edge(nullptr), id(count),
                   joints(std::map<int, float>())
{
    QListWidgetItem::setText(QString::number(id + 1));
    count++;
}

Vertex::Vertex(glm::vec3 pos) : position(pos), half_edge(nullptr), id(count),
                                joints(std::map<int, float>())
{
    QListWidgetItem::setText(QString::number(id + 1));
    count++;
}

Vertex::~Vertex()
{}

int Vertex::get_count() {
    return count;
}

glm::vec3 Vertex::get_position() {
    return position;
}

HalfEdge *Vertex::get_half_edge() {
    return half_edge;
}

int Vertex::get_id() {
    return id;
}

std::map<int, float> Vertex::get_joints() {
    return joints;
}

// Recursive function that is initially passed in the root of
// a Joint skeleton and finds the closest two Joints in that
// skeleton to this Vertex. Returns a vector of size MAX_NUM_JOINT_INFLUENCES
// that stores pointers to the influencing Joints.
void Vertex::get_influencing_joints(std::vector<Joint*> &influencing_joints, Joint *curr_joint) {
    if (influencing_joints.size() < MAX_NUM_JOINT_INFLUENCES) {
        influencing_joints.push_back(curr_joint);
    } else {
        // check if curr_joint is closer to this Vertex than any Joints already in influencing_joints
        glm::vec3 curr_joint_pos = glm::vec3(curr_joint->get_overall_transformation()[3][0],
                                    curr_joint->get_overall_transformation()[3][1],
                                    curr_joint->get_overall_transformation()[3][2]);
        float curr_joint_dist = glm::abs(glm::distance(curr_joint_pos, position));
        int max_influencing_dist = curr_joint_dist; // find Joint in influencing_joints with largest distance from this Vertex
        int max_influencing_dist_joint_index = -1;
        for (int i = 0; i < influencing_joints.size(); i++) {
            Joint *j = influencing_joints.at(i);
            glm::vec3 j_pos = glm::vec3(j->get_overall_transformation()[3][0],
                                        j->get_overall_transformation()[3][1],
                                        j->get_overall_transformation()[3][2]);
            float curr_influencing_joint_dist = glm::abs(glm::distance(j_pos, position));
            if (curr_influencing_joint_dist > max_influencing_dist) {
                max_influencing_dist = curr_influencing_joint_dist;
                max_influencing_dist_joint_index = i;
            }
        }
        if (curr_joint_dist < max_influencing_dist) {   // need to sub in curr_joint into influencing_joints
            influencing_joints.push_back(curr_joint);
            influencing_joints.erase(influencing_joints.begin() + max_influencing_dist_joint_index);
        }
    }

    // recurse on children of curr_joint
    std::vector<Joint*> children = curr_joint->get_children();
    for (Joint *child : children) {
        curr_joint = child;
        get_influencing_joints(influencing_joints, curr_joint);
    }
}

void Vertex::populate_influencing_joints(Joint *curr_joint) {
    std::vector<Joint*> influencing_joints = std::vector<Joint*>();
    get_influencing_joints(influencing_joints, curr_joint);

    // calculate total distance from each influencing Joint to this Vertex
    float total_dist = 0;
    for (Joint *j : influencing_joints) {
        glm::vec3 j_pos = glm::vec3(j->get_overall_transformation()[3][0],
                                    j->get_overall_transformation()[3][1],
                                    j->get_overall_transformation()[3][2]);
        total_dist += glm::abs(glm::distance(j_pos, position));
    }

    // populate joints
    for (Joint *j : influencing_joints) {
        glm::vec3 j_pos = glm::vec3(j->get_overall_transformation()[3][0],
                                    j->get_overall_transformation()[3][1],
                                    j->get_overall_transformation()[3][2]);
        float j_dist_to_vertex = glm::abs(glm::distance(j_pos, position));
        float weight = 1.f - (j_dist_to_vertex / total_dist);
        joints.insert(std::pair<int, float>(j->get_id(), weight));
    }
}

void Vertex::set_half_edge(HalfEdge **he) {
    half_edge = *he;
}

void Vertex::set_pos_x(float x) {
    position.x = x;
}

void Vertex::set_pos_y(float y) {
    position.y = y;
}

void Vertex::set_pos_z(float z) {
    position.z = z;
}

void Vertex::reset_count() {
    count = 0;
}
