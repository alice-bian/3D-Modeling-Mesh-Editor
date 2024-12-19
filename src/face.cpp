#include "face.h"

// initialize static variable
int Face::count = 0;

Face::Face() : color(glm::vec3()), half_edge(nullptr), id(count) {
    QListWidgetItem::setText(QString::number(id + 1));
    count++;
}

Face::Face(glm::vec3 col) : color(col), half_edge(nullptr), id(count) {
    QListWidgetItem::setText(QString::number(id + 1));
    count++;
}

Face::~Face()
{}

int Face::get_count() {
    return count;
}

glm::vec3 Face::get_color() {
    return color;
}

HalfEdge *Face::get_half_edge() {
    return half_edge;
}

int Face::get_id() {
    return id;
}

void Face::set_half_edge(HalfEdge **he) {
    half_edge = *he;
}

void Face::set_col_r(float r) {
    color.r = r;
}

void Face::set_col_g(float g) {
    color.g = g;
}

void Face::set_col_b(float b) {
    color.b = b;
}

void Face::reset_count() {
    count = 0;
}
