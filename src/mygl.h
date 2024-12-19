#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"
#include "vertexdisplay.h"
#include "halfedgedisplay.h"
#include "facedisplay.h"
#include "jointdisplay.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <array>


class MyGL
    : public OpenGLContext
{
    Q_OBJECT

private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram prog_skeleton;// A shader program that transforms vertices based on the joints that influence them

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    Mesh m_mesh;    // The instance of a mesh to be rendered
    VertexDisplay m_vertDisplay; /* Has VBO data representing the position of the currently selected Vertex */
    HalfEdgeDisplay m_halfEdgeDisplay; /* Has VBO data representing the currently selected HalfEdge */
    FaceDisplay m_faceDisplay; /* Has VBO data representing the currently selected Face */
    JointDisplay m_rootJointDisplay; /* Always stores root Joint */
    uPtr<Joint> root_joint; // additional member variable that represents the root Joint of skeleton

    // update selected Vertex's position based on input to SpinBoxes
    void update_vertex_x_pos(double x);
    void update_vertex_y_pos(double y);
    void update_vertex_z_pos(double z);

    // update selected Face's color based on input to SpinBoxes
    void update_face_r_col(double r);
    void update_face_g_col(double g);
    void update_face_b_col(double b);

    // split selected HalfEdge
    void split_half_edge();

    // triangulate selected Face
    void triangulate_face(std::vector<Face**> faces_to_add, std::vector<HalfEdge**> half_edges_to_add);

    // split a HalfEdge for Catmull-Clark subdivision
    // he1 is current HalfEdge that we are splitting
    void split_half_edge_catmull_clark(HalfEdge *he1_ptr, std::map<Face *, Vertex *> &face_centroids);

    // quadrangulate a Face for Catmull-Clark subdivision
    void quadrangulate(Face *f, Vertex* f_centroid);

    // extrude a HalfEdge
    void extrude_half_edge(HalfEdge **curr_half_edge_ptr,
                           Vertex **v3_ptr,
                           Vertex **v4_ptr,
                           HalfEdge **he3_ptr,
                           HalfEdge **he4_ptr,
                           int special_case_num,
                           Vertex **v1_first_ptr,
                           Vertex **v2_first_ptr,
                           HalfEdge ** he1_last);

    // create mesh from imported OBJ file
    void make_mesh_from_file(std::string fname);

    // parses through JSON file to populate root and its descendants
    uPtr<Joint> make_skeleton_from_json(QJsonObject curr_joint);

    // read imported JSON file
    void read_json(std::string fname);

    // skinning function
    void set_bind_matrices(std::array<glm::mat4, 100> &bind_matrices,
                          Joint *curr_joint);
    void set_joint_matrices(std::array<glm::mat4, 100> &joint_matrices,
                                 Joint *curr_joint,
                                 int &num_joints);
    void skin_mesh();

    // update selected Joint's position based on input to SpinBoxes
    void update_joint_x_pos(double x);
    void update_joint_y_pos(double y);
    void update_joint_z_pos(double z);

    // update selected Joint's rotation based on PushButton presses
    void rotate_joint_x(float deg);
    void rotate_joint_y(float deg);
    void rotate_joint_z(float deg);

protected:
    void keyPressEvent(QKeyEvent *e);   // includes additional key press functions for hw06

signals:
    // sends vertices, half-edges, and faces in initial mesh
    void vertices_listed(std::vector<Vertex*> v);
    void half_edges_listed(std::vector<HalfEdge*> he);
    void faces_listed(std::vector<Face*> f);

    // sends vertices, half-edges, and faces to be added to QListWidget in MainWindow
    void add_face_to_QWidgetList(Face*);
    void add_half_edge_to_QWidgetList(HalfEdge*);
    void add_vertex_to_QWidgetList(Vertex*);

    // sends root joint to MainWindow
    void root_joint_constructed(QTreeWidgetItem* node);

};


#endif // MYGL_H
