#include "halfedge.h"

// initialize static variable
int HalfEdge::count = 0;

HalfEdge::HalfEdge() : next_half_edge(nullptr),
                       parallel_half_edge(nullptr),
                       face(nullptr),
                       vertex(nullptr),
                       id(count)
{
    QListWidgetItem::setText(QString::number(id + 1));
    count++;
}

HalfEdge::~HalfEdge()
{}

int HalfEdge::get_count() {
    return count;
}

HalfEdge *HalfEdge::get_next_half_edge() {
    return next_half_edge;
}

HalfEdge *HalfEdge::get_parallel_half_edge() {
    return parallel_half_edge;
}

Face *HalfEdge::get_face() {
    return face;
}

Vertex *HalfEdge::get_vertex() {
    return vertex;
}

int HalfEdge::get_id() {
    return id;
}

void HalfEdge::set_next_half_edge(HalfEdge **he) {
    next_half_edge = *he;
}

void HalfEdge::set_parallel_half_edge(HalfEdge **he) {
    parallel_half_edge = *he;
}

void HalfEdge::set_face(Face **f) {
    face = *f;
}

void HalfEdge::set_vertex(Vertex **v) {
    vertex = *v;
}

void HalfEdge::reset_count() {
    count = 0;
}
