#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace std;

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this), prog_skeleton(this),
      m_glCamera(),
      m_mesh(this),
      m_vertDisplay(this),
      m_halfEdgeDisplay(this),
      m_faceDisplay(this),
      m_rootJointDisplay(this),
      root_joint(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL() {
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_mesh.destroy();
    m_vertDisplay.destroy();
    m_halfEdgeDisplay.destroy();
    m_faceDisplay.destroy();
    m_rootJointDisplay.destroy();
}

void MyGL::initializeGL() {
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create the instances of Cylinder and Sphere.
    m_geomSquare.create();
    m_mesh.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    // Create and set up the skeleton shader
    prog_skeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // populate all lists
    m_vertDisplay.create();
    m_halfEdgeDisplay.create();
    m_faceDisplay.create();
    m_rootJointDisplay.create();

    // build vector of Vertex pointers, emit to MainWindow
    std::vector<Vertex*> vertex_list_pointers = m_mesh.get_vertex_pointers();
    emit vertices_listed(vertex_list_pointers);

    // build vector of HalfEdge pointers, emit to MainWindow
    std::vector<HalfEdge*> half_edge_list_pointers = m_mesh.get_half_edge_pointers();
    emit half_edges_listed(half_edge_list_pointers);

    // build vector of Face pointers, emit to MainWindow
    std::vector<Face*> face_list_pointers = m_mesh.get_face_pointers();
    emit faces_listed(face_list_pointers);

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h) {
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    prog_skeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL() {
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    prog_skeleton.setViewProjMatrix(m_glCamera.getViewProj());
    prog_skeleton.setCamPos(m_glCamera.eye);

//#define NOPE
#ifndef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::mat4(1.0f);
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setModelMatrix(model);
    m_progFlat.setModelMatrix(model);
    prog_skeleton.setModelMatrix(model);
    //Draw mesh
    if (m_mesh.get_is_skinned()) {
        prog_skeleton.draw(m_mesh);
    } else {
        m_progLambert.draw(m_mesh);
    }

    // highlight selected vertex
    if (m_vertDisplay.get_represented_vertex() != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_vertDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    // highlight selected half-edge
    if (m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_halfEdgeDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    // highlight selected face
    if (m_faceDisplay.get_represented_face() != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_faceDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    // highlight selected joint
    if (m_rootJointDisplay.get_represented_joint() != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_rootJointDisplay);
        glEnable(GL_DEPTH_TEST);
    }

#endif
}

void MyGL::keyPressEvent(QKeyEvent *e) {
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        // change to rotate about theta
        m_glCamera.RotateTheta(-amount);
        /*m_glCamera.RotateAboutUp(-amount);*/
    } else if (e->key() == Qt::Key_Left) {
        // change to rotate about theta
        m_glCamera.RotateTheta(amount);
        /*m_glCamera.RotateAboutUp(amount);*/
    } else if (e->key() == Qt::Key_Up) {
        // change to rotate about phi
        m_glCamera.RotatePhi(amount);
        /*m_glCamera.RotateAboutRight(-amount);*/
    } else if (e->key() == Qt::Key_Down) {
        // change to rotate about phi
        m_glCamera.RotatePhi(-amount);
        /*m_glCamera.RotateAboutRight(amount);*/
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    }

    // select and highlight next half-edge of currently selected half-edge
    else if (e->key() == Qt::Key_N) {
        if (m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
            QListWidgetItem *i = m_halfEdgeDisplay.get_represented_half_edge()->get_next_half_edge();
            m_halfEdgeDisplay.destroy();
            m_halfEdgeDisplay.set_represented_half_edge(static_cast<HalfEdge*>(i));
            m_halfEdgeDisplay.create();
            update();
        }
    }
    // select and highlight symmetric half-edge of currently selected half-edge
    else if (e->key() == Qt::Key_M) {
        if (m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
            QListWidgetItem *i = m_halfEdgeDisplay.get_represented_half_edge()->get_parallel_half_edge();
            m_halfEdgeDisplay.destroy();
            m_halfEdgeDisplay.set_represented_half_edge(static_cast<HalfEdge*>(i));
            m_halfEdgeDisplay.create();
            update();
        }
    }
    // select and highlight face of currently selected half-edge
    else if (e->key() == Qt::Key_F) {
        if (m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
            QListWidgetItem *i = m_halfEdgeDisplay.get_represented_half_edge()->get_face();
            m_faceDisplay.destroy();
            m_faceDisplay.set_represented_face(static_cast<Face*>(i));
            m_faceDisplay.create();
            update();
        }
    }
    // select and highlight vertex of currently selected half-edge
    else if (e->key() == Qt::Key_V) {
        if (m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
            QListWidgetItem *i = m_halfEdgeDisplay.get_represented_half_edge()->get_vertex();
            m_vertDisplay.destroy();
            m_vertDisplay.set_represented_vertex(static_cast<Vertex*>(i));
            m_vertDisplay.create();
            update();
        }
    }
    // select and highlight half-edge of currently selected face
    else if ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_H) {
        if (m_faceDisplay.get_represented_face() != nullptr) {
            QListWidgetItem *i = m_faceDisplay.get_represented_face()->get_half_edge();
            m_halfEdgeDisplay.destroy();
            m_halfEdgeDisplay.set_represented_half_edge(static_cast<HalfEdge*>(i));
            m_halfEdgeDisplay.create();
            update();
        }
    }
    // select and highlight half-edge of currently selected vertex
    else if (e->key() == Qt::Key_H) {
        if (m_vertDisplay.get_represented_vertex() != nullptr) {
            QListWidgetItem *i = m_vertDisplay.get_represented_vertex()->get_half_edge();
            m_halfEdgeDisplay.destroy();
            m_halfEdgeDisplay.set_represented_half_edge(static_cast<HalfEdge*>(i));
            m_halfEdgeDisplay.create();
            update();
        }
    }

    // zoom in/out
    else if ((e->modifiers() & Qt::CTRL) && e->key() == Qt::Key_Minus) {    // Command -
        m_glCamera.Zoom(amount / 5.f);
    } else if ((e->modifiers() & Qt::CTRL) && e->key() == Qt::Key_Equal) {  // Command +
        m_glCamera.Zoom(-amount / 5.f);
    }

    // reset
    else if ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_R) {
        m_glCamera.Reset();
    }

    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::update_vertex_x_pos(double x) {
    m_vertDisplay.get_represented_vertex()->set_pos_x(x);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::update_vertex_y_pos(double y) {
    m_vertDisplay.get_represented_vertex()->set_pos_y(y);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::update_vertex_z_pos(double z) {
    m_vertDisplay.get_represented_vertex()->set_pos_z(z);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::update_face_r_col(double r) {
    m_faceDisplay.get_represented_face()->set_col_r(r);
    m_mesh.destroy();
    m_mesh.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    update();
}

void MyGL::update_face_g_col(double g) {
    m_faceDisplay.get_represented_face()->set_col_g(g);
    m_mesh.destroy();
    m_mesh.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    update();
}

void MyGL::update_face_b_col(double b) {
    m_faceDisplay.get_represented_face()->set_col_b(b);
    m_mesh.destroy();
    m_mesh.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    update();
}

void MyGL::split_half_edge() {
    // find endpoint vertices of he1
    HalfEdge *curr_half_edge2 = m_halfEdgeDisplay.get_represented_half_edge()->get_next_half_edge();
    HalfEdge *prev_half_edge2 = curr_half_edge2;
    while(curr_half_edge2 != m_halfEdgeDisplay.get_represented_half_edge()) {
        prev_half_edge2 = curr_half_edge2;
        curr_half_edge2 = curr_half_edge2->get_next_half_edge();
    }
    Vertex *v2 = prev_half_edge2->get_vertex();
    glm::vec3 v2_pos = glm::vec3(v2->get_position());
    Vertex *v1 = m_halfEdgeDisplay.get_represented_half_edge()->get_vertex();
    glm::vec3 v1_pos = glm::vec3(v1->get_position());

    // find endpoint vertices of he2
    HalfEdge *curr_half_edge1 = m_halfEdgeDisplay.get_represented_half_edge()->get_parallel_half_edge()->get_next_half_edge();
    HalfEdge *prev_half_edge1 = curr_half_edge1;
    while(curr_half_edge1 != m_halfEdgeDisplay.get_represented_half_edge()->get_parallel_half_edge()) {
        prev_half_edge1 = curr_half_edge1;
        curr_half_edge1 = curr_half_edge1->get_next_half_edge();
    }

    // create new midpoint Vertex
    uPtr<Vertex> midpoint = mkU<Vertex>((v1_pos + v2_pos) / 2.f);
    Vertex *midpoint_ptr = midpoint.get();
    m_mesh.add_vertex(midpoint);

    // create two new HalfEdges
    HalfEdge *he1 = m_halfEdgeDisplay.get_represented_half_edge();
    HalfEdge *he2 = m_halfEdgeDisplay.get_represented_half_edge()->get_parallel_half_edge();
    uPtr<HalfEdge> he1b = mkU<HalfEdge>();
    HalfEdge *he1b_ptr = he1b.get();
    m_mesh.add_half_edge(he1b);
    uPtr<HalfEdge> he2b = mkU<HalfEdge>();
    HalfEdge *he2b_ptr = he2b.get();
    m_mesh.add_half_edge(he2b);

    // relink everything
    midpoint_ptr->set_half_edge(&he1);                   // midpoint vertex
    v1->set_half_edge(&prev_half_edge1);                 // v1
    v2->set_half_edge(&prev_half_edge2);                 // v2

    he1b_ptr->set_vertex(&v1);                               // HalfEdge he1b
    Face *he1b_face = he1->get_face();
    he1b_ptr->set_face(&he1b_face);
    HalfEdge *he1b_next = he1->get_next_half_edge();
    he1b_ptr->set_next_half_edge(&he1b_next);
    he1b_ptr->set_parallel_half_edge(&he2);

    he2b_ptr->set_vertex(&v2);                               // HalfEdge he2b
    Face *he2b_face = he2->get_face();
    he2b_ptr->set_face(&he2b_face);
    HalfEdge *he2b_next = he2->get_next_half_edge();
    he2b_ptr->set_next_half_edge(&he2b_next);
    he2b_ptr->set_parallel_half_edge(&he1);

    he1->set_vertex(&midpoint_ptr);                      // HalfEdge he1
    he1->set_next_half_edge(&he1b_ptr);
    he1->set_parallel_half_edge(&he2b_ptr);

    he2->set_vertex(&midpoint_ptr);                      // HalfEdge he2
    he2->set_next_half_edge(&he2b_ptr);
    he2->set_parallel_half_edge(&he1b_ptr);

    // add new Vertex and HalfEdges to GUI list
    emit add_vertex_to_QWidgetList(midpoint_ptr);
    emit add_half_edge_to_QWidgetList(he1b_ptr);
    emit add_half_edge_to_QWidgetList(he2b_ptr);

    // update display
    m_mesh.destroy();
    m_mesh.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::triangulate_face(std::vector<Face**> faces_to_add, std::vector<HalfEdge**> half_edges_to_add) {
    // define initial state
    Face *face1_ptr = m_faceDisplay.get_represented_face();
    HalfEdge *he_0_ptr = face1_ptr->get_half_edge();

    // create new Face
    uPtr<Face> face2 = mkU<Face>(face1_ptr->get_color());
    Face *face2_ptr = face2.get();
    m_mesh.add_face(face2);

    // create two new HalfEdges
    uPtr<HalfEdge> he_a = mkU<HalfEdge>();
    HalfEdge *he_a_ptr = he_a.get();
    m_mesh.add_half_edge(he_a);
    uPtr<HalfEdge> he_b = mkU<HalfEdge>();
    HalfEdge *he_b_ptr = he_b.get();
    m_mesh.add_half_edge(he_b);

    // relink everything
    Vertex *triangle_vertex1 = he_0_ptr->get_vertex();      // HalfEdge he_a
    he_a_ptr->set_vertex(&triangle_vertex1);
    Vertex *triangle_vertex3 = he_0_ptr->get_next_half_edge()->get_next_half_edge()->get_vertex();      // HalfEdge he_b
    he_b_ptr->set_vertex(&triangle_vertex3);
    he_a_ptr->set_parallel_half_edge(&he_b_ptr);
    he_b_ptr->set_parallel_half_edge(&he_a_ptr);
    he_a_ptr->set_face(&face1_ptr);
    he_0_ptr->get_next_half_edge()->set_face(&face2_ptr);   // HalfEdge he_0_next
    he_0_ptr->get_next_half_edge()->get_next_half_edge()->set_face(&face2_ptr); // HalfEdge he_0_next_next
    he_b_ptr->set_face(&face1_ptr);
    face2_ptr->set_half_edge(&he_a_ptr);                    // Face face2
    HalfEdge *he_0_next_next_next_ptr = he_0_ptr->get_next_half_edge()->get_next_half_edge()->get_next_half_edge();
    he_b_ptr->set_next_half_edge(&he_0_next_next_next_ptr);
    he_0_ptr->get_next_half_edge()->get_next_half_edge()->set_next_half_edge(&he_a_ptr);
    HalfEdge *he_0_next = he_0_ptr->get_next_half_edge();
    he_a_ptr->set_next_half_edge(&he_0_next);
    he_0_ptr->set_next_half_edge(&he_b_ptr);                // HalfEdge he_0

    // add new Face and HalfEdges to GUI list
    faces_to_add.push_back(&face2_ptr);
    half_edges_to_add.push_back(&he_a_ptr);
    half_edges_to_add.push_back(&he_b_ptr);

    // update display
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::split_half_edge_catmull_clark(HalfEdge *he1_ptr,
                                         std::map<Face*, Vertex*> &face_centroids) {
    // find endpoint vertices of he1
    HalfEdge *he1 = he1_ptr;
    Vertex *v2 = he1->get_parallel_half_edge()->get_vertex();
    glm::vec3 v2_pos = glm::vec3(v2->get_position());
    Vertex *v1 = he1->get_vertex();
    glm::vec3 v1_pos = glm::vec3(v1->get_position());

    HalfEdge *he2 = he1->get_parallel_half_edge();

    // create new smoothed midpoint Vertex
    Face *f1 = he1->get_face();
    glm::vec3 f1_centroid_pos = face_centroids[f1]->get_position();
    Face *f2 = he2->get_face();
    glm::vec3 f2_centroid_pos = face_centroids[f2]->get_position();
    uPtr<Vertex> midpoint = mkU<Vertex>((v1_pos + v2_pos + f1_centroid_pos + f2_centroid_pos) / 4.f);
    Vertex *midpoint_ptr = midpoint.get();
    m_mesh.add_vertex(midpoint);

    // create two new HalfEdges
    uPtr<HalfEdge> he1b = mkU<HalfEdge>();
    HalfEdge *he1b_ptr = he1b.get();
    m_mesh.add_half_edge(he1b);
    uPtr<HalfEdge> he2b = mkU<HalfEdge>();
    HalfEdge *he2b_ptr = he2b.get();
    m_mesh.add_half_edge(he2b);

    // relink everything
    midpoint_ptr->set_half_edge(&he1);                   // midpoint vertex
    v1->set_half_edge(&he1b_ptr);                        // v1
    v2->set_half_edge(&he2b_ptr);                        // v2

    he1b_ptr->set_vertex(&v1);                           // HalfEdge he1b
    Face *he1b_face = he1->get_face();
    he1b_ptr->set_face(&he1b_face);
    HalfEdge *he1b_next = he1->get_next_half_edge();
    he1b_ptr->set_next_half_edge(&he1b_next);
    he1b_ptr->set_parallel_half_edge(&he2);

    he2b_ptr->set_vertex(&v2);                           // HalfEdge he2b
    Face *he2b_face = he2->get_face();
    he2b_ptr->set_face(&he2b_face);
    HalfEdge *he2b_next = he2->get_next_half_edge();
    he2b_ptr->set_next_half_edge(&he2b_next);
    he2b_ptr->set_parallel_half_edge(&he1);

    he1->set_vertex(&midpoint_ptr);                      // HalfEdge he1
    he1->set_next_half_edge(&he1b_ptr);
    he1->set_parallel_half_edge(&he2b_ptr);

    he2->set_vertex(&midpoint_ptr);                      // HalfEdge he2
    he2->set_next_half_edge(&he2b_ptr);
    he2->set_parallel_half_edge(&he1b_ptr);

    // add new Vertex and HalfEdges to GUI list
    emit add_vertex_to_QWidgetList(midpoint_ptr);
    emit add_half_edge_to_QWidgetList(he1b_ptr);
    emit add_half_edge_to_QWidgetList(he2b_ptr);
}

void MyGL::quadrangulate(Face *f, Vertex* f_centroid) {
    // initial setup
    HalfEdge *starting_half_edge = f->get_half_edge();
    HalfEdge *curr_half_edge = f->get_half_edge();
    HalfEdge *curr_half_edge_prev = f->get_half_edge(); // save HalfEdge before current one
    Vertex *he2_vertex = nullptr;  // save Vertex that he2 needs to point to
    while (curr_half_edge_prev->get_next_half_edge()->get_next_half_edge() != starting_half_edge) {
        curr_half_edge_prev = curr_half_edge_prev->get_next_half_edge();
    }
    he2_vertex = curr_half_edge_prev->get_vertex();
    curr_half_edge_prev = curr_half_edge_prev->get_next_half_edge();
    HalfEdge *prev_he1 = nullptr;   // save previous iteration's he1 for setting he2's syms
    HalfEdge *first_he2 = nullptr;  // save first he2 for setting last iteration's he1 syms
    bool first = true;
    float color_adjustment = 20.f;

    // save starting HalfEdge of next quad-face loop
    HalfEdge *next_curr_half_edge_prev = curr_half_edge->get_next_half_edge();

    // quadrangulate each subface
    while (first || (curr_half_edge != starting_half_edge)) {
        // create two new HalfEdges
        uPtr<HalfEdge> he1 = mkU<HalfEdge>();
        HalfEdge *he1_ptr = he1.get();
        m_mesh.add_half_edge(he1);
        emit add_half_edge_to_QWidgetList(he1_ptr);
        uPtr<HalfEdge> he2 = mkU<HalfEdge>();
        HalfEdge *he2_ptr = he2.get();
        m_mesh.add_half_edge(he2);
        emit add_half_edge_to_QWidgetList(he2_ptr);

        // create new Face
        Face *subface_ptr = nullptr;
        if (first) {    // use f on first iteration
            subface_ptr = f;
        } else {
            uPtr<Face> subface = mkU<Face>(f->get_color() - glm::vec3(color_adjustment, color_adjustment, color_adjustment));
            subface_ptr = subface.get();
            m_mesh.add_face(subface);
            emit add_face_to_QWidgetList(subface_ptr);
        }

        // relink everything
        curr_half_edge->set_next_half_edge(&he1_ptr);
        curr_half_edge->set_face(&subface_ptr);

        he1_ptr->set_next_half_edge(&he2_ptr);
        he1_ptr->set_face(&subface_ptr);
        he1_ptr->set_vertex(&f_centroid);
        // on last iteration, need to set first he2(aka he1 of this iteration)'s syms
        if (next_curr_half_edge_prev->get_next_half_edge() == starting_half_edge) {
            he1_ptr->set_parallel_half_edge(&first_he2);
            he1_ptr->get_parallel_half_edge()->set_parallel_half_edge(&he1_ptr);
        }

        he2_ptr->set_next_half_edge(&curr_half_edge_prev);
        if (!first) {
            he2_ptr->set_parallel_half_edge(&prev_he1);
            he2_ptr->get_parallel_half_edge()->set_parallel_half_edge(&he2_ptr);    // set previous he1's sym
        }
        he2_ptr->set_face(&subface_ptr);
        he2_ptr->set_vertex(&he2_vertex);

        he2_ptr->get_next_half_edge()->set_face(&subface_ptr);

        subface_ptr->set_half_edge(&curr_half_edge);

        f_centroid->set_half_edge(&he1_ptr);

        he2_ptr->get_vertex()->set_half_edge(&he2_ptr);

        // set up for new iteration
        if (first) {
            first_he2 = he2_ptr;
        }
        prev_he1 = he1_ptr;
        he2_vertex = curr_half_edge->get_vertex();
        curr_half_edge_prev = next_curr_half_edge_prev;
        curr_half_edge = curr_half_edge_prev->get_next_half_edge();
        next_curr_half_edge_prev = next_curr_half_edge_prev->get_next_half_edge()->get_next_half_edge();
        first = false;
        color_adjustment += 20.f;
    }
}

void MyGL::extrude_half_edge(HalfEdge **curr_half_edge_ptr,
                             Vertex **v3_ptr,
                             Vertex **v4_ptr,
                             HalfEdge **he3_ptr,
                             HalfEdge **he4_ptr,
                             int special_case_num,
                             Vertex **v1_first_ptr,
                             Vertex **v2_first_ptr,
                             HalfEdge ** he1_last) {
    // initial variable setup
    HalfEdge *he1 = *curr_half_edge_ptr;
    Vertex *v1 = he1->get_vertex();
    if (special_case_num == 2) {    // Edge Case: On last HalfEdge to extrude, it has already been brought up by other HalfEdges!
        he1 = *he1_last;
        v1 = *v2_first_ptr;
    }
    HalfEdge *he2 = he1->get_parallel_half_edge();
    Vertex *v2 = he2->get_vertex();

    // create new Vertices (if necessary)
    Vertex *v3 = *v3_ptr;
    Vertex *v4 = *v4_ptr;
    if (v3 == nullptr) {   // avoid creating overlapping Vertex from previous extrude iteration
        // calculate Face normal
        Vertex *v1_next = he1->get_next_half_edge()->get_vertex();
        if (special_case_num == 1) {
            v1_next = *v1_first_ptr;
        }
        glm::vec3 edge1 = v1_next->get_position() - v1->get_position();
        glm::vec3 edge2 = v2->get_position() - v1->get_position();
        glm::vec3 v3_f_nor = glm::cross(edge1, edge2);
        glm::vec3 v3_offset = 2.f * glm::normalize(v3_f_nor); // calculate HalfEdge offset

        uPtr<Vertex> v3_unique = mkU<Vertex>(v1->get_position() + v3_offset);
        v3 = v3_unique.get();
        m_mesh.add_vertex(v3_unique);
    }
    if (v4 == nullptr) {
        // calculate Face normal
        HalfEdge *he1_prev = he1;
        while (he1_prev->get_next_half_edge()->get_next_half_edge() != he1) {
            he1_prev = he1_prev->get_next_half_edge();
        }
        Vertex *v2_prev = he1_prev->get_vertex();
        glm::vec3 edge1 = v1->get_position() - v2->get_position();
        glm::vec3 edge2 = v2_prev->get_position() - v2->get_position();
        glm::vec3 v4_f_nor = glm::cross(edge1, edge2);
        glm::vec3 v4_offset = 2.f * glm::normalize(v4_f_nor); // calculate HalfEdge offset

        uPtr<Vertex> v4_unique = mkU<Vertex>(v2->get_position() + v4_offset);
        v4 = v4_unique.get();
        m_mesh.add_vertex(v4_unique);
    }

    // relink he1
    he1->set_vertex(&v3);
    v3->set_half_edge(&he1);
    // find previous HalfEdge of he1 for relinking
    HalfEdge *he1_prev_half_edge = he1->get_next_half_edge();
    while(he1_prev_half_edge->get_next_half_edge() != he1) {
        he1_prev_half_edge = he1_prev_half_edge->get_next_half_edge();
    }
    he1_prev_half_edge->set_vertex(&v4);
    v4->set_half_edge(&he1_prev_half_edge);

    // create two new HalfEdges
    uPtr<HalfEdge> he1b_unique = mkU<HalfEdge>();
    HalfEdge *he1b = he1b_unique.get();
    m_mesh.add_half_edge(he1b_unique);
    uPtr<HalfEdge> he2b_unique = mkU<HalfEdge>();
    HalfEdge *he2b = he2b_unique.get();
    m_mesh.add_half_edge(he2b_unique);

    // relink he1b and he2b
    he1->set_parallel_half_edge(&he1b);
    he2->set_parallel_half_edge(&he2b);
    he1b->set_parallel_half_edge(&he1);
    he2b->set_parallel_half_edge(&he2);
    he1b->set_vertex(&v4);
    he2b->set_vertex(&v1);

    // create new Face and another two HalfEdges
    uPtr<Face> f_unique = mkU<Face>(glm::vec3(100.f, 100.f, 100.f));
    Face *f = f_unique.get();
    m_mesh.add_face(f_unique);
    uPtr<HalfEdge> he3_unique = mkU<HalfEdge>();
    HalfEdge *he3 = he3_unique.get();
    m_mesh.add_half_edge(he3_unique);
    uPtr<HalfEdge> he4_unique = mkU<HalfEdge>();
    HalfEdge *he4 = he4_unique.get();
    m_mesh.add_half_edge(he4_unique);

    // relink f, he3, and he4
    f->set_half_edge(&he1b);
    he1b->set_face(&f);
    he2b->set_face(&f);
    he3->set_face(&f);
    he4->set_face(&f);
    he3->set_vertex(&v3);
    he4->set_vertex(&v2);
    he1b->set_next_half_edge(&he4);
    he4->set_next_half_edge(&he2b);
    he2b->set_next_half_edge(&he3);
    he3->set_next_half_edge(&he1b);

    // add new Face, HalfEdges, and Vertices to GUI list
    emit add_face_to_QWidgetList(f);
    emit add_half_edge_to_QWidgetList(he1b);
    emit add_half_edge_to_QWidgetList(he2b);
    *v3_ptr = v3;
    *v4_ptr = v4;
    *he3_ptr = he3;
    *he4_ptr = he4;
}

void MyGL::make_mesh_from_file(string fname) {
    // clear mesh
    m_mesh.clear();

    if (fname.size() == 0) { // no OBJ file selected
        return;
    }

    ifstream filename(fname);
    string line;
    std::vector<Vertex*> vertices = std::vector<Vertex*>(); // store Vertex pointers for this mesh
    std::map<string, HalfEdge*> half_edge_syms = std::map<string, HalfEdge*>();   // maps to determine symmetric HalfEdges
    while (!filename.eof()) {
        getline(filename, line);

        // get Vertexes
        if (line.substr(0, 2) == "v ") {
            // extract position of Vertex
            istringstream vertex_pos(line);
            string char_v, x, y, z;
            vertex_pos >> char_v >> x >> y >> z;    // >> operator parses by space

            // create Vertex
            uPtr<Vertex> v = mkU<Vertex>(glm::vec3(stof(x), stof(y), stof(z)));
            Vertex *v_ptr = v.get();
            m_mesh.add_vertex(v);
            emit add_vertex_to_QWidgetList(v_ptr);
            vertices.push_back(v_ptr);
        }

        // get Faces and create HalfEdges while looping through
        else if (line.substr(0, 2) == "f ") {
            // generate random color for face
            glm::vec3 f_color = glm::vec3();
            f_color.x = rand() % 256;
            f_color.y = rand() % 256;
            f_color.z = rand() % 256;

            // create Face
            uPtr<Face> f = mkU<Face>(f_color);
            Face *f_ptr = f.get();
            m_mesh.add_face(f);
            emit add_face_to_QWidgetList(f_ptr);

            // parse through rest of line to extract and store Vertex IDs for this face
            istringstream face(line);
            string f_char, tok;
            face >> f_char;
            std::vector<int> f_vertex_ids = std::vector<int>();
            while (face >> tok) {   // store each #/#/# cluster in tok
                // extract Vertex ID from tok (first # in #/#/#)
                int first_slash_index = tok.find('/');
                string vertex_id = tok.substr(0, first_slash_index);
                f_vertex_ids.push_back(stof(vertex_id) - 1);    // -1 is to account for indexing in vertices vector
            }

            // build HalfEdges and link Vertices on this Face
            HalfEdge *he_prev = nullptr;
            HalfEdge *he_first = nullptr;
            for (int i = 0; i < f_vertex_ids.size(); i++) {
                Vertex *v_ptr = vertices.at(f_vertex_ids.at(i));

                // create HalfEdge
                uPtr<HalfEdge> he = mkU<HalfEdge>();
                HalfEdge *he_ptr = he.get();
                m_mesh.add_half_edge(he);
                emit add_half_edge_to_QWidgetList(he_ptr);

                // link all of HE's pointers
                he_ptr->set_face(&f_ptr);
                he_ptr->set_vertex(&v_ptr);
                if (i != 0) {   // on first iteration, he_prev is a nullptr
                    he_prev->set_next_half_edge(&he_ptr);
                } else {        // save first HE for last iteration
                    he_first = he_ptr;
                    f_ptr->set_half_edge(&he_first);
                }
                // on last iteration, need to set current HE's next pointer to first HE
                if (i == f_vertex_ids.size() - 1) {
                    he_ptr->set_next_half_edge(&he_first);
                }

                // set HE's symmetric pointer
                int starting_vertex_ID = i - 1;
                if (i == 0) {
                    starting_vertex_ID = f_vertex_ids.size() - 1;
                }
                int ending_vertex_ID = i;
                string he_endpoints = to_string(f_vertex_ids.at(ending_vertex_ID)) + " to " + to_string(f_vertex_ids.at(starting_vertex_ID));
                auto search = half_edge_syms.find(he_endpoints);
                if (search != half_edge_syms.end()) {  // check if symmetric HalfEdge has already been created
                    // set sym pointers
                    HalfEdge *sym_he = half_edge_syms[he_endpoints];
                    he_ptr->set_parallel_half_edge(&sym_he);
                    sym_he->set_parallel_half_edge(&he_ptr);
                } else {    // if not, then add to map
                    he_endpoints = to_string(f_vertex_ids.at(starting_vertex_ID)) + " to " + to_string(f_vertex_ids.at(ending_vertex_ID));
                    half_edge_syms.insert(std::pair<string, HalfEdge*>(he_endpoints, he_ptr));
                }

                // link vertex's HE poiner
                v_ptr->set_half_edge(&he_ptr);

                // set up for next iteration
                he_prev = he_ptr;
            }
        }
    }

    m_mesh.destroy();
    m_mesh.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

uPtr<Joint> MyGL::make_skeleton_from_json(QJsonObject curr_joint) {
    QString name = curr_joint["name"].toString();       // extract name
    QJsonArray pos_arr = curr_joint["pos"].toArray();   // extract position array
    glm::vec3 pos = glm::vec3(pos_arr[0].toDouble(),
                              pos_arr[1].toDouble(),
                              pos_arr[2].toDouble());
    QJsonArray rotation_arr = curr_joint["rot"].toArray();   // extract rotation quat
    glm::quat rotation = glm::angleAxis((float)(glm::radians(rotation_arr[0].toDouble())),  // angle (radians)
                                        glm::vec3(rotation_arr[1].toDouble(), // axis
                                        rotation_arr[2].toDouble(),
                                        rotation_arr[3].toDouble()));

    // create Joint
    uPtr<Joint> j = mkU<Joint>(name, pos, rotation); // create Joint
    Joint *j_ptr = j.get();

    // extract children
    QJsonArray children_arr = curr_joint["children"].toArray();
    std::vector<uPtr<Joint>> children = std::vector<uPtr<Joint>>();
    for (int i = 0; i < children_arr.size(); i++) {
        QJsonObject child = children_arr[i].toObject();
        children.push_back(make_skeleton_from_json(child));

    }
    for (int i = 0; i < children.size(); i++) {
        j_ptr->add_child(std::move(children.at(i))).set_parent(j_ptr);
    }

    return j;
}

void MyGL::read_json(std::string fname) {
    QFile file;
    file.setFileName(QString::fromStdString(fname));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    qWarning() << val;  // for testing
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        QJsonObject root = obj["root"].toObject();
        root_joint = make_skeleton_from_json(root);
        Joint *root_joint_ptr = root_joint.get();
        m_rootJointDisplay.set_represented_joint(root_joint_ptr);
        emit root_joint_constructed(root_joint_ptr);
    }
}

void MyGL::set_bind_matrices(std::array<glm::mat4, 100> &bind_matrices,
                            Joint *curr_joint) {
    bind_matrices[curr_joint->get_id()] = curr_joint->set_bind_matrix();
    for (const uPtr<Joint>& child : curr_joint->children) {
        curr_joint = child.get();
        set_bind_matrices(bind_matrices, curr_joint);
    }
}

void MyGL::set_joint_matrices(std::array<glm::mat4, 100> &joint_matrices,
                             Joint *curr_joint,
                             int &num_joints) {
    joint_matrices[curr_joint->get_id()] = curr_joint->get_overall_transformation();
    num_joints++;
    for (const uPtr<Joint>& child : curr_joint->children) {
        curr_joint = child.get();
        set_joint_matrices(joint_matrices, curr_joint, num_joints);
    }
}

void MyGL::skin_mesh() {
    // set is_skinned bool in Mesh class
    m_mesh.set_is_skinned(true);
    std::vector<Vertex*> vertices = m_mesh.get_vertex_pointers();
    for (Vertex* v : vertices) {
        // find closest two joints that influence each vertex
        v->populate_influencing_joints(m_rootJointDisplay.get_represented_joint());
    }

    // set up shader uniforms (bind matrix and overall transformation matrix for each Joint)
    int num_joints = 0;
    std::array<glm::mat4, 100> bind_matrices = std::array<glm::mat4, 100>();
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_bind_matrices(bind_matrices, root_joint.get());
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setBindMatrix(bind_matrices, num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);

    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();
}

void MyGL::update_joint_x_pos(double x) {
    m_rootJointDisplay.get_selected_joint()->set_pos_x(x);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}

void MyGL::update_joint_y_pos(double y) {
    m_rootJointDisplay.get_selected_joint()->set_pos_y(y);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}

void MyGL::update_joint_z_pos(double z) {
    m_rootJointDisplay.get_selected_joint()->set_pos_z(z);
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}

void MyGL::rotate_joint_x(float deg) {
    glm::quat q_5deg = glm::angleAxis((float)(glm::radians(deg)), glm::vec3(1, 0, 0));
    m_rootJointDisplay.get_selected_joint()->set_rot(q_5deg
                                                     * m_rootJointDisplay.get_selected_joint()->get_rot());
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}

void MyGL::rotate_joint_y(float deg) {
    glm::quat q_5deg = glm::angleAxis((float)(glm::radians(deg)), glm::vec3(0, 1, 0));
    m_rootJointDisplay.get_selected_joint()->set_rot(q_5deg
                                                     * m_rootJointDisplay.get_selected_joint()->get_rot());
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}

void MyGL::rotate_joint_z(float deg) {
    glm::quat q_5deg = glm::angleAxis((float)(glm::radians(deg)), glm::vec3(0, 0, 1));
    m_rootJointDisplay.get_selected_joint()->set_rot(q_5deg
                                                     * m_rootJointDisplay.get_selected_joint()->get_rot());
    m_mesh.destroy();
    m_mesh.create();
    m_vertDisplay.destroy();
    m_vertDisplay.create();
    m_halfEdgeDisplay.destroy();
    m_halfEdgeDisplay.create();
    m_faceDisplay.destroy();
    m_faceDisplay.create();
    m_rootJointDisplay.destroy();
    m_rootJointDisplay.create();
    update();

    // set up overall transformation matrix for each Joint
    int num_joints = 0;
    std::array<glm::mat4, 100> joint_matrices = std::array<glm::mat4, 100>();
    set_joint_matrices(joint_matrices, root_joint.get(), num_joints);
    prog_skeleton.setJointMatrix(joint_matrices, num_joints);
}
