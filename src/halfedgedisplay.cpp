#include "halfedgedisplay.h"
#include <iostream>

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext* mp_context) : Drawable(mp_context),
                                                              represented_half_edge(nullptr)
{}

GLenum HalfEdgeDisplay::drawMode() {
    return GL_LINES;
}

void HalfEdgeDisplay::create() {
    // only want to display if represented_half_edge is not null (i.e. something is selected)!
    if (represented_half_edge == nullptr) {
        return;
    }

    // find endpoint vertices
    Vertex *yellowP = represented_half_edge->get_vertex();
    Vertex *redP = represented_half_edge->get_parallel_half_edge()->get_vertex();

    glm::vec4 endpoint1 = glm::vec4(redP->get_position(), 1.f);
    glm::vec4 endpoint2 = glm::vec4(yellowP->get_position(), 1.f);

    std::vector<glm::vec4> pos = {endpoint1, endpoint2};
    std::vector<glm::vec4> col = {glm::vec4(1.f, 0.f, 0.f, 0.f),    // red
                                  glm::vec4(1.f, 1.f, 0.f, 0.f)};   // yellow
    std::vector<GLuint> idx = {0, 1};

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

HalfEdge *HalfEdgeDisplay::get_represented_half_edge() {
    return represented_half_edge;
}

void HalfEdgeDisplay::set_represented_half_edge(HalfEdge *he) {
    represented_half_edge = he;
}
