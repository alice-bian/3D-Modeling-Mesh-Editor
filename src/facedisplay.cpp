#include "facedisplay.h"

FaceDisplay::FaceDisplay(OpenGLContext* mp_context) : Drawable(mp_context),
                                                      represented_face(nullptr)
{}

GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

void FaceDisplay::create() {
    // only want to display if represented_face is not null (i.e. something is selected)!
    if (represented_face == nullptr) {
        return;
    }

    // fill pos
    std::vector<glm::vec4> pos = std::vector<glm::vec4>();
    HalfEdge *curr_half_edge = represented_face->get_half_edge();
    Vertex *curr_vertex = curr_half_edge->get_vertex();
    Vertex *starting_vertex = curr_vertex;
    glm::vec4 curr_vertex_pos = glm::vec4(curr_vertex->get_position(), 1.f);
    pos.push_back(curr_vertex_pos);

    curr_half_edge = curr_half_edge->get_next_half_edge();
    curr_vertex = curr_half_edge->get_vertex();

    while(curr_vertex != starting_vertex) {
        curr_vertex_pos = glm::vec4(curr_vertex->get_position(), 1.f);
        pos.push_back(curr_vertex_pos);
        curr_half_edge = curr_half_edge->get_next_half_edge();
        curr_vertex = curr_half_edge->get_vertex();
    }

    // fill col
    std::vector<glm::vec4> col = std::vector<glm::vec4>();
    glm::vec3 color = represented_face->get_color();
    color = 255.f - color;  // invert color
    color /= 255.f;
    glm::vec4 face_col = glm::vec4(color, 0.f);
    for (int i = 0; i < pos.size(); i++) {
        col.push_back(face_col);
    }

    // fill idx
    // EX: idx = {0 1 1 2 2 3 3 4 4 0} for pentagon
    std::vector<GLuint> idx = std::vector<GLuint>();
    for (int i = 0; i < pos.size(); i++) {
        if (i == pos.size() - 1) {
            idx.push_back((GLuint) i);
            idx.push_back((GLuint) 0);
        } else {
            idx.push_back((GLuint) i);
            idx.push_back((GLuint) i + 1);
        }
    }

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

Face *FaceDisplay::get_represented_face() {
    return represented_face;
}

void FaceDisplay::set_represented_face(Face *f) {
    represented_face = f;
}
