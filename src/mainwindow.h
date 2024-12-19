#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include "mygl.h"


namespace Ui
{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void on_actionCamera_Controls_triggered();

private:
    Ui::MainWindow *ui;

public slots:
    // adding initial vertices, half-edges, and faces to QListWidget
    void slot_add_vertices_to_QListWidget(std::vector<Vertex*>);
    void slot_add_half_edges_to_QListWidget(std::vector<HalfEdge*>);
    void slot_add_faces_to_QListWidget(std::vector<Face*>);

    // adding additional vertices, half-edges, and faces to QListWidget
    void slot_add_vertex_to_QListWidget(Vertex*);
    void slot_add_half_edge_to_QListWidget(HalfEdge*);
    void slot_add_face_to_QListWidget(Face*);


    // highlight selected Vertex, HalfEdge, and/or Face
    void slot_set_selected_vertex(QListWidgetItem *i);
    void slot_set_selected_half_edge(QListWidgetItem *i);
    void slot_set_selected_face(QListWidgetItem *i);

    // update selected Vertex's position based on input to SpinBoxes
    void slot_update_vertex_x_pos(double);
    void slot_update_vertex_y_pos(double);
    void slot_update_vertex_z_pos(double);

    // update selected Face's color based on input to SpinBoxes
    void slot_update_face_r_col(double);
    void slot_update_face_g_col(double);
    void slot_update_face_b_col(double);

    // split selected HalfEdge
    void slot_split_half_edge();

    // triangulate selected Face
    void slot_triangulate_face();

    // Catmull-Clark subdivide selected mesh
    void slot_catmull_clark_subdivision();

    // extrude selected Face
    void slot_extrude_face();

    // load OBJ file
    void slot_import_obj_file();

    // laod JSON file
    void slot_load_json_file();

    // skinning function
    void slot_skin_mesh();

    // Joint-related slots
    void slot_add_joint_to_QTreeWidget(QTreeWidgetItem*);
    void slot_set_selected_joint(QTreeWidgetItem*);

    // interactive skeleton slots
    void slot_update_joint_x_pos(double);
    void slot_update_joint_y_pos(double);
    void slot_update_joint_z_pos(double);
    void slot_rotate_joint_x_CCW();
    void slot_rotate_joint_x_CW();
    void slot_rotate_joint_y_CCW();
    void slot_rotate_joint_y_CW();
    void slot_rotate_joint_z_CCW();
    void slot_rotate_joint_z_CW();
};


#endif // MAINWINDOW_H
