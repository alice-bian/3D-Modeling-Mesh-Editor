#include "joint.h"
#include <iostream>

// initialize static variable
int Joint::count = 0;

// default constructor
Joint::Joint(QString name, glm::vec3 pos, glm::quat rotation) : name(name),
                                                                parent(nullptr),
                                                                children(std::vector<uPtr<Joint>>()),
                                                                pos(pos),
                                                                rotation(rotation),
                                                                bind_matrix(glm::mat4()),
                                                                id(count) {
    QTreeWidgetItem::setText(0, name);
    count++;
}

// destructor
Joint::~Joint()
{}

/**
  * Function that assigns j2 to this.
  *
  * Return: A reference to the newly assigned Joint.
  */
Joint& Joint::operator=(const Joint &j2) {
    name = j2.name;
    parent = j2.parent;
    children = std::vector<uPtr<Joint>>();
    pos = j2.pos;
    rotation = j2.rotation;
    bind_matrix = j2.bind_matrix;

    return *this;
}

/**
  * Function that adds a given uPtr as a child to this Joint.
  *
  * Return: A reference to the newly added uPtr Joint.
  */
Joint& Joint::add_child(uPtr<Joint> child) {
    // add child to Tree Widget
    QTreeWidgetItem::addChild(child.get());

    // place this new uPtr in children vector
    children.push_back(std::move(child));

    // return reference to newly added Joint
    return *(children.back());
}

glm::mat4 Joint::get_local_transformation() {
    // convert quat to rotation matrix and pos to translation matrix, multiply them together
    glm::mat4 translation_matrix = glm::translate(glm::mat4(), pos);
    return translation_matrix * glm::toMat4(rotation);
}

glm::mat4 Joint::get_overall_transformation() {
    // traverse up tree and multiply together all local transformation matrices
    if (!parent) {
        return get_local_transformation();
    }
    return parent->get_overall_transformation() * get_local_transformation();
}

glm::mat4 Joint::set_bind_matrix() {
    bind_matrix = glm::inverse(get_overall_transformation());
    return bind_matrix;
}

int Joint::get_id() {
    return id;
}

glm::vec3 Joint::get_pos() {
    return pos;
}

std::vector<Joint*> Joint::get_children() {
    std::vector<Joint*> c = std::vector<Joint*>();
    for (const uPtr<Joint> &child : children) {
        c.push_back(child.get());
    }
    return c;
}

void Joint::set_parent(Joint *j) {
    parent = j;
}

void Joint::set_pos_x(float x) {
    pos.x = x;
}

void Joint::set_pos_y(float y) {
    pos.y = y;
}

void Joint::set_pos_z(float z) {
    pos.z = z;
}

glm::quat Joint::get_rot() {
    return rotation;
}

void Joint::set_rot(glm::quat q) {
    rotation = q;
}
