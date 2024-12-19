#include <iostream>
#include "mesh.h"


Mesh::Mesh(OpenGLContext *mp_context) : Drawable(mp_context),
                                        vertices(std::vector<uPtr<Vertex>>()),
                                        half_edges(std::vector<uPtr<HalfEdge>>()),
                                        faces(std::vector<uPtr<Face>>()),
                                        is_skinned(false)
{
    create_cube_mesh();
}

void Mesh::create_cube_mesh() {
    /* 8 vertices of cube */
    uPtr<Vertex> v1 = mkU<Vertex>(glm::vec3(-0.5f, -0.5f, 0.5f));   // intersection of front, left, bottom faces
    uPtr<Vertex> v2 = mkU<Vertex>(glm::vec3(0.5f, -0.5f, 0.5f));    // intersection of front, right, bottom faces
    uPtr<Vertex> v3 = mkU<Vertex>(glm::vec3(0.5f, 0.5f, 0.5f));     // intersection of front, right, top faces
    uPtr<Vertex> v4 = mkU<Vertex>(glm::vec3(-0.5f, 0.5f, 0.5f));    // intersection of front, left, top faces
    uPtr<Vertex> v5 = mkU<Vertex>(glm::vec3(0.5f, -0.5f, -0.5f));   // intersection of back, right, bottom faces
    uPtr<Vertex> v6 = mkU<Vertex>(glm::vec3(-0.5f, -0.5f, -0.5f));  // intersection of back, left, bottom faces
    uPtr<Vertex> v7 = mkU<Vertex>(glm::vec3(-0.5f, 0.5f, -0.5f));   // intersection of back, left, top faces
    uPtr<Vertex> v8 = mkU<Vertex>(glm::vec3(0.5f, 0.5f, -0.5f));    // intersection of back, right, top faces

    vertices.push_back(std::move(v1));      // put vertices in vector
    vertices.push_back(std::move(v2));
    vertices.push_back(std::move(v3));
    vertices.push_back(std::move(v4));
    vertices.push_back(std::move(v5));
    vertices.push_back(std::move(v6));
    vertices.push_back(std::move(v7));
    vertices.push_back(std::move(v8));

    Vertex *p_v1 = vertices.at(0).get();    // pointers to vertices
    Vertex *p_v2 = vertices.at(1).get();
    Vertex *p_v3 = vertices.at(2).get();
    Vertex *p_v4 = vertices.at(3).get();
    Vertex *p_v5 = vertices.at(4).get();
    Vertex *p_v6 = vertices.at(5).get();
    Vertex *p_v7 = vertices.at(6).get();
    Vertex *p_v8 = vertices.at(7).get();

    /* 6 faces of cube */
    uPtr<Face> left = mkU<Face>(glm::vec3(255.f, 0.f, 0.f));        // negative x-axis
    uPtr<Face> right = mkU<Face>(glm::vec3(0.f, 255.f, 0.f));       // positive x-axis
    uPtr<Face> top = mkU<Face>(glm::vec3(0.f, 0.f, 255.f));         // positive y-axis
    uPtr<Face> bottom = mkU<Face>(glm::vec3(255.f, 255.f, 0.f));    // negative y-axis
    uPtr<Face> front = mkU<Face>(glm::vec3(0.f, 255.f, 255.f));     // positive z-axis
    uPtr<Face> back = mkU<Face>(glm::vec3(255.f, 0.f, 255.f));      // negative z-axis

    faces.push_back(std::move(left));   // put faces in vector
    faces.push_back(std::move(right));
    faces.push_back(std::move(top));
    faces.push_back(std::move(bottom));
    faces.push_back(std::move(front));
    faces.push_back(std::move(back));

    Face *p_left = faces.at(0).get();   // pointers to faces
    Face *p_right = faces.at(1).get();
    Face *p_top = faces.at(2).get();
    Face *p_bottom = faces.at(3).get();
    Face *p_front = faces.at(4).get();
    Face *p_back = faces.at(5).get();

    /* half-edges of cube (4 per face) */
    uPtr<HalfEdge> left1 = mkU<HalfEdge>();     // left face, left half-edge (loooking at cube)
    uPtr<HalfEdge> left2 = mkU<HalfEdge>();     // left face, bottom half-edge
    uPtr<HalfEdge> left3 = mkU<HalfEdge>();     // left face, right half-edge
    uPtr<HalfEdge> left4 = mkU<HalfEdge>();     // left face, top half-edge
    uPtr<HalfEdge> right1 = mkU<HalfEdge>();    // right face
    uPtr<HalfEdge> right2 = mkU<HalfEdge>();
    uPtr<HalfEdge> right3 = mkU<HalfEdge>();
    uPtr<HalfEdge> right4 = mkU<HalfEdge>();
    uPtr<HalfEdge> top1 = mkU<HalfEdge>();      // top face
    uPtr<HalfEdge> top2 = mkU<HalfEdge>();
    uPtr<HalfEdge> top3 = mkU<HalfEdge>();
    uPtr<HalfEdge> top4 = mkU<HalfEdge>();
    uPtr<HalfEdge> bottom1 = mkU<HalfEdge>();   // bottom face
    uPtr<HalfEdge> bottom2 = mkU<HalfEdge>();
    uPtr<HalfEdge> bottom3 = mkU<HalfEdge>();
    uPtr<HalfEdge> bottom4 = mkU<HalfEdge>();
    uPtr<HalfEdge> front1 = mkU<HalfEdge>();    // front face
    uPtr<HalfEdge> front2 = mkU<HalfEdge>();
    uPtr<HalfEdge> front3 = mkU<HalfEdge>();
    uPtr<HalfEdge> front4 = mkU<HalfEdge>();
    uPtr<HalfEdge> back1 = mkU<HalfEdge>();     // back face
    uPtr<HalfEdge> back2 = mkU<HalfEdge>();
    uPtr<HalfEdge> back3 = mkU<HalfEdge>();
    uPtr<HalfEdge> back4 = mkU<HalfEdge>();

    half_edges.push_back(std::move(left1));     // put half-edges in vector
    half_edges.push_back(std::move(left2));
    half_edges.push_back(std::move(left3));
    half_edges.push_back(std::move(left4));
    half_edges.push_back(std::move(right1));
    half_edges.push_back(std::move(right2));
    half_edges.push_back(std::move(right3));
    half_edges.push_back(std::move(right4));
    half_edges.push_back(std::move(top1));
    half_edges.push_back(std::move(top2));
    half_edges.push_back(std::move(top3));
    half_edges.push_back(std::move(top4));
    half_edges.push_back(std::move(bottom1));
    half_edges.push_back(std::move(bottom2));
    half_edges.push_back(std::move(bottom3));
    half_edges.push_back(std::move(bottom4));
    half_edges.push_back(std::move(front1));
    half_edges.push_back(std::move(front2));
    half_edges.push_back(std::move(front3));
    half_edges.push_back(std::move(front4));
    half_edges.push_back(std::move(back1));
    half_edges.push_back(std::move(back2));
    half_edges.push_back(std::move(back3));
    half_edges.push_back(std::move(back4));

                                                    // pointers to half-edge
    HalfEdge *p_left1 = half_edges.at(0).get();     // left face, left half-edge (loooking at cube)
    HalfEdge *p_left2 = half_edges.at(1).get();
    HalfEdge *p_left3 = half_edges.at(2).get();
    HalfEdge *p_left4 = half_edges.at(3).get();
    HalfEdge *p_right1 = half_edges.at(4).get();    // right face
    HalfEdge *p_right2 = half_edges.at(5).get();
    HalfEdge *p_right3 = half_edges.at(6).get();
    HalfEdge *p_right4 = half_edges.at(7).get();
    HalfEdge *p_top1 = half_edges.at(8).get();      // top face
    HalfEdge *p_top2 = half_edges.at(9).get();
    HalfEdge *p_top3 = half_edges.at(10).get();
    HalfEdge *p_top4 = half_edges.at(11).get();
    HalfEdge *p_bottom1 = half_edges.at(12).get();  // bottom face
    HalfEdge *p_bottom2 = half_edges.at(13).get();
    HalfEdge *p_bottom3 = half_edges.at(14).get();
    HalfEdge *p_bottom4 = half_edges.at(15).get();
    HalfEdge *p_front1 = half_edges.at(16).get();   // front face
    HalfEdge *p_front2 = half_edges.at(17).get();
    HalfEdge *p_front3 = half_edges.at(18).get();
    HalfEdge *p_front4 = half_edges.at(19).get();
    HalfEdge *p_back1 = half_edges.at(20).get();    // back face
    HalfEdge *p_back2 = half_edges.at(21).get();
    HalfEdge *p_back3 = half_edges.at(22).get();
    HalfEdge *p_back4 = half_edges.at(23).get();

    /* Linking vertices, half-edges, and faces */
    (*(vertices.at(0).get())).set_half_edge(&p_front1); // set half-edges for vertices
    (*(vertices.at(1).get())).set_half_edge(&p_front2);
    (*(vertices.at(2).get())).set_half_edge(&p_front3);
    (*(vertices.at(3).get())).set_half_edge(&p_front4);
    (*(vertices.at(4).get())).set_half_edge(&p_back1);
    (*(vertices.at(5).get())).set_half_edge(&p_back2);
    (*(vertices.at(6).get())).set_half_edge(&p_back3);
    (*(vertices.at(7).get())).set_half_edge(&p_back4);

    (*(faces.at(0).get())).set_half_edge(&p_left1);     // set half-edges for faces
    (*(faces.at(1).get())).set_half_edge(&p_right1);
    (*(faces.at(2).get())).set_half_edge(&p_top1);
    (*(faces.at(3).get())).set_half_edge(&p_bottom1);
    (*(faces.at(4).get())).set_half_edge(&p_front1);
    (*(faces.at(5).get())).set_half_edge(&p_back1);

    (*(half_edges.at(0).get())).set_next_half_edge(&p_left2);   // set next half-edge for half-edges
    (*(half_edges.at(1).get())).set_next_half_edge(&p_left3);
    (*(half_edges.at(2).get())).set_next_half_edge(&p_left4);
    (*(half_edges.at(3).get())).set_next_half_edge(&p_left1);
    (*(half_edges.at(4).get())).set_next_half_edge(&p_right2);
    (*(half_edges.at(5).get())).set_next_half_edge(&p_right3);
    (*(half_edges.at(6).get())).set_next_half_edge(&p_right4);
    (*(half_edges.at(7).get())).set_next_half_edge(&p_right1);
    (*(half_edges.at(8).get())).set_next_half_edge(&p_top2);
    (*(half_edges.at(9).get())).set_next_half_edge(&p_top3);
    (*(half_edges.at(10).get())).set_next_half_edge(&p_top4);
    (*(half_edges.at(11).get())).set_next_half_edge(&p_top1);
    (*(half_edges.at(12).get())).set_next_half_edge(&p_bottom2);
    (*(half_edges.at(13).get())).set_next_half_edge(&p_bottom3);
    (*(half_edges.at(14).get())).set_next_half_edge(&p_bottom4);
    (*(half_edges.at(15).get())).set_next_half_edge(&p_bottom1);
    (*(half_edges.at(16).get())).set_next_half_edge(&p_front2);
    (*(half_edges.at(17).get())).set_next_half_edge(&p_front3);
    (*(half_edges.at(18).get())).set_next_half_edge(&p_front4);
    (*(half_edges.at(19).get())).set_next_half_edge(&p_front1);
    (*(half_edges.at(20).get())).set_next_half_edge(&p_back2);
    (*(half_edges.at(21).get())).set_next_half_edge(&p_back3);
    (*(half_edges.at(22).get())).set_next_half_edge(&p_back4);
    (*(half_edges.at(23).get())).set_next_half_edge(&p_back1);

    (*(half_edges.at(0).get())).set_parallel_half_edge(&p_back3);   // set symmetric half-edge for half-edges
    (*(half_edges.at(1).get())).set_parallel_half_edge(&p_bottom1);
    (*(half_edges.at(2).get())).set_parallel_half_edge(&p_front1);
    (*(half_edges.at(3).get())).set_parallel_half_edge(&p_top1);
    (*(half_edges.at(4).get())).set_parallel_half_edge(&p_front3);
    (*(half_edges.at(5).get())).set_parallel_half_edge(&p_bottom3);
    (*(half_edges.at(6).get())).set_parallel_half_edge(&p_back1);
    (*(half_edges.at(7).get())).set_parallel_half_edge(&p_top3);
    (*(half_edges.at(8).get())).set_parallel_half_edge(&p_left4);
    (*(half_edges.at(9).get())).set_parallel_half_edge(&p_front4);
    (*(half_edges.at(10).get())).set_parallel_half_edge(&p_right4);
    (*(half_edges.at(11).get())).set_parallel_half_edge(&p_back4);
    (*(half_edges.at(12).get())).set_parallel_half_edge(&p_left2);
    (*(half_edges.at(13).get())).set_parallel_half_edge(&p_back2);
    (*(half_edges.at(14).get())).set_parallel_half_edge(&p_right2);
    (*(half_edges.at(15).get())).set_parallel_half_edge(&p_front2);
    (*(half_edges.at(16).get())).set_parallel_half_edge(&p_left3);
    (*(half_edges.at(17).get())).set_parallel_half_edge(&p_bottom4);
    (*(half_edges.at(18).get())).set_parallel_half_edge(&p_right1);
    (*(half_edges.at(19).get())).set_parallel_half_edge(&p_top2);
    (*(half_edges.at(20).get())).set_parallel_half_edge(&p_right3);
    (*(half_edges.at(21).get())).set_parallel_half_edge(&p_bottom2);
    (*(half_edges.at(22).get())).set_parallel_half_edge(&p_left1);
    (*(half_edges.at(23).get())).set_parallel_half_edge(&p_top4);

    (*(half_edges.at(0).get())).set_face(&p_left);    // set faces for half-edges
    (*(half_edges.at(1).get())).set_face(&p_left);
    (*(half_edges.at(2).get())).set_face(&p_left);
    (*(half_edges.at(3).get())).set_face(&p_left);
    (*(half_edges.at(4).get())).set_face(&p_right);
    (*(half_edges.at(5).get())).set_face(&p_right);
    (*(half_edges.at(6).get())).set_face(&p_right);
    (*(half_edges.at(7).get())).set_face(&p_right);
    (*(half_edges.at(8).get())).set_face(&p_top);
    (*(half_edges.at(9).get())).set_face(&p_top);
    (*(half_edges.at(10).get())).set_face(&p_top);
    (*(half_edges.at(11).get())).set_face(&p_top);
    (*(half_edges.at(12).get())).set_face(&p_bottom);
    (*(half_edges.at(13).get())).set_face(&p_bottom);
    (*(half_edges.at(14).get())).set_face(&p_bottom);
    (*(half_edges.at(15).get())).set_face(&p_bottom);
    (*(half_edges.at(16).get())).set_face(&p_front);
    (*(half_edges.at(17).get())).set_face(&p_front);
    (*(half_edges.at(18).get())).set_face(&p_front);
    (*(half_edges.at(19).get())).set_face(&p_front);
    (*(half_edges.at(20).get())).set_face(&p_back);
    (*(half_edges.at(21).get())).set_face(&p_back);
    (*(half_edges.at(22).get())).set_face(&p_back);
    (*(half_edges.at(23).get())).set_face(&p_back);

    (*(half_edges.at(0).get())).set_vertex(&p_v6);  // set vertices for half-edges
    (*(half_edges.at(1).get())).set_vertex(&p_v1);
    (*(half_edges.at(2).get())).set_vertex(&p_v4);
    (*(half_edges.at(3).get())).set_vertex(&p_v7);
    (*(half_edges.at(4).get())).set_vertex(&p_v2);
    (*(half_edges.at(5).get())).set_vertex(&p_v5);
    (*(half_edges.at(6).get())).set_vertex(&p_v8);
    (*(half_edges.at(7).get())).set_vertex(&p_v3);
    (*(half_edges.at(8).get())).set_vertex(&p_v4);
    (*(half_edges.at(9).get())).set_vertex(&p_v3);
    (*(half_edges.at(10).get())).set_vertex(&p_v8);
    (*(half_edges.at(11).get())).set_vertex(&p_v7);
    (*(half_edges.at(12).get())).set_vertex(&p_v6);
    (*(half_edges.at(13).get())).set_vertex(&p_v5);
    (*(half_edges.at(14).get())).set_vertex(&p_v2);
    (*(half_edges.at(15).get())).set_vertex(&p_v1);
    (*(half_edges.at(16).get())).set_vertex(&p_v1);
    (*(half_edges.at(17).get())).set_vertex(&p_v2);
    (*(half_edges.at(18).get())).set_vertex(&p_v3);
    (*(half_edges.at(19).get())).set_vertex(&p_v4);
    (*(half_edges.at(20).get())).set_vertex(&p_v5);
    (*(half_edges.at(21).get())).set_vertex(&p_v6);
    (*(half_edges.at(22).get())).set_vertex(&p_v7);
    (*(half_edges.at(23).get())).set_vertex(&p_v8);
}

std::vector<GLuint> Mesh::triangulate(int starting_vertex_num, int num_sides) {
    std::vector<GLuint> face_idx = std::vector<GLuint>();
    for (int i = starting_vertex_num; i < starting_vertex_num + num_sides - 2; i++) {
        face_idx.push_back(starting_vertex_num);
        face_idx.push_back((GLuint) (i + 1));
        face_idx.push_back((GLuint) (i + 2));
    }
    return face_idx;
}

void Mesh::create() {
    std::vector<glm::vec4> pos = std::vector<glm::vec4>();
    std::vector<glm::vec4> nor = std::vector<glm::vec4>();
    std::vector<glm::vec4> col = std::vector<glm::vec4>();
    std::vector<GLuint> idx = std::vector<GLuint>();

    int starting_vertex_num = 0;
    int curr_vertex_num = 0;
    for (int i = 0; i < faces.size(); i++) {
        Face *curr_face = faces.at(i).get();

        // obtain first Vertex
        HalfEdge *curr_half_edge = curr_face->get_half_edge();
        Vertex *starting_vertex = curr_half_edge->get_vertex();
        Vertex *curr_vertex = curr_half_edge->get_vertex();
        curr_vertex_num++;

        // push all Vertex positions on this Face into pos
        glm::vec4 vbo_pos = glm::vec4((curr_vertex->get_position()), 1.f);
        pos.push_back(vbo_pos);

        // push Face normal vector into nor
        glm::vec3 v1_pos = curr_vertex->get_position(); // Will always be starting vertex
        HalfEdge *h2 = curr_half_edge->get_next_half_edge();
        glm::vec3 v2_pos = h2->get_vertex()->get_position();
        glm::vec3 edge1 = v2_pos - v1_pos;
        Vertex *v3 = curr_half_edge->get_parallel_half_edge()->get_vertex();
        glm::vec3 v3_pos = v3->get_position();
        glm::vec3 edge2 = v3_pos - v1_pos;
        glm::vec3 vbo_nor = glm::cross(edge1, edge2);
        glm::vec3 v1_pos_temp = v1_pos;         // edge case: cross product is 0
        glm::vec3 v2_pos_temp = v2_pos;
        while (vbo_nor[0] == 0 && vbo_nor[1] == 0 && vbo_nor[2] == 0 && (h2->get_vertex() != starting_vertex)) {
            v1_pos_temp = v2_pos_temp;
            h2 = h2->get_next_half_edge();
            v2_pos_temp = h2->get_vertex()->get_position();
            edge1 = v2_pos_temp - v1_pos_temp;
            vbo_nor = glm::cross(edge1, edge2);
        }
        nor.push_back(glm::vec4((vbo_nor), 0.f));

        // push Face color vector into col
        glm::vec4 vbo_col = glm::vec4((curr_face->get_color()), 0.f);
        vbo_col /= 255.f;    // normalize color
        col.push_back(vbo_col);

        // obtain next Vertex in loop
        curr_half_edge = curr_half_edge->get_next_half_edge();
        curr_vertex = curr_half_edge->get_vertex();

        // triangulate remaining vertices of this Face
        while (curr_vertex != starting_vertex) { // trace through half-edge loop on this face
            // push onto pos
            vbo_pos = glm::vec4((curr_vertex->get_position()), 1.f);
            pos.push_back(vbo_pos);

            // push onto nor
            v3_pos = v1_pos;
            v1_pos = v2_pos;
            h2 = curr_half_edge->get_next_half_edge();
            v2_pos = h2->get_vertex()->get_position();
            edge1 = v2_pos - v1_pos;
            edge2 = v3_pos - v1_pos;
            vbo_nor = glm::cross(edge1, edge2);
            v1_pos_temp = v1_pos;       // edge case: cross product is 0
            v2_pos_temp = v2_pos;
            while (vbo_nor[0] == 0 && vbo_nor[1] == 0 && vbo_nor[2] == 0 && (h2->get_vertex() != starting_vertex)) {
                v1_pos_temp = v2_pos_temp;
                h2 = h2->get_next_half_edge();
                v2_pos_temp = h2->get_vertex()->get_position();
                edge1 = v2_pos_temp - v1_pos_temp;
                vbo_nor = glm::cross(edge1, edge2);
            }
            nor.push_back(glm::vec4((vbo_nor), 0.f));

            // push onto col
            col.push_back(vbo_col);

            // obtain next Vertex in loop
            curr_half_edge = (*curr_half_edge).get_next_half_edge();
            curr_vertex = (*curr_half_edge).get_vertex();
            curr_vertex_num++;
        }

        // fill in idx buffer for this face
        std::vector<GLuint> face_idx = triangulate(starting_vertex_num,
                                                   curr_vertex_num - starting_vertex_num);
        for (int j = 0; j < face_idx.size(); j++) {
            idx.push_back(face_idx[j]);
        }

        // reset starting vertex
        starting_vertex_num = curr_vertex_num;
    }

    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    // add in for skeleton shader
    if (is_skinned) {
        std::vector<glm::vec2> joint_weights = std::vector<glm::vec2>();
        std::vector<glm::ivec2> joint_IDs = std::vector<glm::ivec2>();

        for (int i = 0; i < faces.size(); i++) {
            // obtain starting Vertex
            Face *curr_face = faces.at(i).get();
            HalfEdge *starting_half_edge = curr_face->get_half_edge();
            HalfEdge *curr_half_edge = curr_face->get_half_edge();
            Vertex *curr_vertex = curr_half_edge->get_vertex();
            bool first = true;

            // find Joint weights and IDs of all Vertices in this Face's loop
            while (first || curr_half_edge != starting_half_edge) {
                std::map<int, float> joints = curr_vertex->get_joints();
                std::vector<int> joint_id = std::vector<int>();
                std::vector<float> joint_w = std::vector<float>();
                for (const auto &kv : joints) {
                    joint_id.push_back(kv.first);
                    joint_w.push_back(kv.second);
                }
                glm::vec2 w = glm::vec2(joint_w[0], joint_w[1]);
                joint_weights.push_back(w);
                glm::ivec2 id = glm::ivec2(joint_id[0], joint_id[1]);
                joint_IDs.push_back(id);

                // obtain next Vertex in loop
                curr_half_edge = curr_half_edge->get_next_half_edge();
                curr_vertex = curr_half_edge->get_vertex();
                first = false;
            }
        }

        generateJointWeights();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufJointWeights);
        mp_context->glBufferData(GL_ARRAY_BUFFER, joint_weights.size() * sizeof(glm::vec2), joint_weights.data(), GL_STATIC_DRAW);

        generateJointIDs();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufJointIDs);
        mp_context->glBufferData(GL_ARRAY_BUFFER, joint_IDs.size() * sizeof(glm::ivec2), joint_IDs.data(), GL_STATIC_DRAW);
    }
}

std::vector<Vertex*> Mesh::get_vertex_pointers() {
    std::vector<Vertex*> vertex_list_pointers = std::vector<Vertex*>();
    for (int i = 0; i < vertices.size(); i++) {
        vertex_list_pointers.push_back(vertices.at(i).get());
    }
    return vertex_list_pointers;
}

std::vector<HalfEdge*> Mesh::get_half_edge_pointers() {
    std::vector<HalfEdge*> half_edge_list_pointers = std::vector<HalfEdge*>();
    for (int i = 0; i < half_edges.size(); i++) {
        half_edge_list_pointers.push_back(half_edges.at(i).get());
    }
    return half_edge_list_pointers;
}

std::vector<Face*> Mesh::get_face_pointers() {
    std::vector<Face*> face_list_pointers = std::vector<Face*>();
    for (int i = 0; i < faces.size(); i++) {
        face_list_pointers.push_back(faces.at(i).get());
    }
    return face_list_pointers;
}

Vertex& Mesh::add_vertex(uPtr<Vertex> &vert) {
    // place this uPtr in Vertex vector
    vertices.push_back(std::move(vert));

    // return reference to newly added Vertex
    return *(vertices.back());
}

HalfEdge& Mesh::add_half_edge(uPtr<HalfEdge> &he) {
    // place this uPtr in HalfEdge vector
    half_edges.push_back(std::move(he));

    // return reference to newly added HalfEdge
    return *(half_edges.back());
}

Face& Mesh::add_face(uPtr<Face> &f) {
    // place this uPtr in Face vector
    faces.push_back(std::move(f));

    // return reference to newly added Face
    return *(faces.back());
}

void Mesh::clear() {
    vertices.clear();
    half_edges.clear();
    faces.clear();

    Vertex::reset_count();
    HalfEdge::reset_count();
    Face::reset_count();
}

bool Mesh::get_is_skinned() {
    return is_skinned;
}

void Mesh::set_is_skinned(bool skinned) {
    is_skinned = skinned;
}
