#include "vertexdisplay.h"
#include <iostream>

VertexDisplay::VertexDisplay(OpenGLContext* mp_context) : Drawable(mp_context), represented_vertex(nullptr)
{}

GLenum VertexDisplay::drawMode() {
    return GL_POINTS;
}

void VertexDisplay::create() {
    // only want to display if represented_vertex is not null (i.e. something is selected)!
    if (represented_vertex == nullptr) {
        return;
    }

    std::vector<glm::vec4> pos = {glm::vec4((*represented_vertex).get_position(), 1.f)};
    std::vector<glm::vec4> col = {glm::vec4(1.f, 1.f, 1.f, 0.f)};   // white
    std::vector<GLuint> idx = {0};

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

Vertex *VertexDisplay::get_represented_vertex() {
    return represented_vertex;
}

void VertexDisplay::set_represented_vertex(Vertex *v) {
    represented_vertex = v;
}
