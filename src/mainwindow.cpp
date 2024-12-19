#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <iostream>
#include <set>

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    // connect Vertex, HalfEdge, and Face list pointers from Mesh classes to QListWidgets
    connect(ui->mygl,
            SIGNAL(vertices_listed(std::vector<Vertex*>)),
            this,
            SLOT(slot_add_vertices_to_QListWidget(std::vector<Vertex*>)));
    connect(ui->mygl,
            SIGNAL(half_edges_listed(std::vector<HalfEdge*>)),
            this,
            SLOT(slot_add_half_edges_to_QListWidget(std::vector<HalfEdge*>)));
    connect(ui->mygl,
            SIGNAL(faces_listed(std::vector<Face*>)),
            this,
            SLOT(slot_add_faces_to_QListWidget(std::vector<Face*>)));

    connect(ui->mygl, SIGNAL(add_vertex_to_QWidgetList(Vertex*)), this, SLOT(slot_add_vertex_to_QListWidget(Vertex*)));
    connect(ui->mygl, SIGNAL(add_half_edge_to_QWidgetList(HalfEdge*)), this, SLOT(slot_add_half_edge_to_QListWidget(HalfEdge*)));
    connect(ui->mygl, SIGNAL(add_face_to_QWidgetList(Face*)), this, SLOT(slot_add_face_to_QListWidget(Face*)));

    // connect and highlight selected Vertex, HalfEdge, and/or Face
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_set_selected_vertex(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_set_selected_half_edge(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_set_selected_face(QListWidgetItem*)));

    // connect spin boxes (vertex pos and face col)
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_vertex_x_pos(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_vertex_y_pos(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_vertex_z_pos(double)));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_face_r_col(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_face_g_col(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_face_b_col(double)));

    // connect buttons
    connect(ui->splitHalfEdgeButton, SIGNAL(clicked()), this, SLOT(slot_split_half_edge()));
    connect(ui->triangulateFaceButton, SIGNAL(clicked()), this, SLOT(slot_triangulate_face()));
    connect(ui->catmullClarkSubdivisionButton, SIGNAL(clicked()), this, SLOT(slot_catmull_clark_subdivision()));
    connect(ui->extrudeFaceButton, SIGNAL(clicked()), this, SLOT(slot_extrude_face()));
    connect(ui->importOBJFileButton, SIGNAL(clicked()), this, SLOT(slot_import_obj_file()));
    connect(ui->loadJSONFileButton, SIGNAL(clicked()), this, SLOT(slot_load_json_file()));
    connect(ui->skinMeshButton, SIGNAL(clicked()), this, SLOT(slot_skin_mesh()));

    // connect selected joint
    connect(ui->jointTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slot_set_selected_joint(QTreeWidgetItem*)));

    // connect Tree Widget
    connect(ui->mygl, SIGNAL(root_joint_constructed(QTreeWidgetItem*)), this, SLOT(slot_add_joint_to_QTreeWidget(QTreeWidgetItem*)));

    // connect interactive skeleton Widgets
    connect(ui->jointPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_joint_x_pos(double)));
    connect(ui->jointPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_joint_y_pos(double)));
    connect(ui->jointPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_update_joint_z_pos(double)));
    connect(ui->jointRotateXCCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_x_CCW()));
    connect(ui->jointRotateXCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_x_CW()));
    connect(ui->jointRotateYCCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_y_CCW()));
    connect(ui->jointRotateYCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_y_CW()));
    connect(ui->jointRotateZCCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_z_CCW()));
    connect(ui->jointRotateZCWButton, SIGNAL(clicked()), this, SLOT(slot_rotate_joint_z_CW()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionQuit_triggered() {
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered() {
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_add_vertices_to_QListWidget(std::vector<Vertex*> vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        ui->vertsListWidget->addItem(vertices.at(i));
    }
}

void MainWindow::slot_add_half_edges_to_QListWidget(std::vector<HalfEdge*> half_edges) {
    for (int i = 0; i < half_edges.size(); i++) {
        ui->halfEdgesListWidget->addItem(half_edges.at(i));
    }
}

void MainWindow::slot_add_faces_to_QListWidget(std::vector<Face*> faces) {
    for (int i = 0; i < faces.size(); i++) {
        ui->facesListWidget->addItem(faces.at(i));
    }
}

void MainWindow::slot_add_vertex_to_QListWidget(Vertex* v) {
    ui->vertsListWidget->addItem(v);
}

void MainWindow::slot_add_half_edge_to_QListWidget(HalfEdge* he) {
    ui->halfEdgesListWidget->addItem(he);
}

void MainWindow::slot_add_face_to_QListWidget(Face* f) {
    ui->facesListWidget->addItem(f);
}

void MainWindow::slot_set_selected_vertex(QListWidgetItem *i) {
    if (ui->mygl->m_vertDisplay.get_represented_vertex() != nullptr)
    {
        ui->vertPosXSpinBox->setValue(ui->mygl->m_vertDisplay.get_represented_vertex()->get_position().x);
        ui->vertPosYSpinBox->setValue(ui->mygl->m_vertDisplay.get_represented_vertex()->get_position().y);
        ui->vertPosZSpinBox->setValue(ui->mygl->m_vertDisplay.get_represented_vertex()->get_position().z);
    }
    ui->mygl->m_vertDisplay.destroy(); // ONLY CHANGES VBO DATA!
    i = (Vertex*) i;
    ui->mygl->m_vertDisplay.set_represented_vertex(static_cast<Vertex*>(i));
    ui->mygl->m_vertDisplay.create();
    ui->mygl->update();
}

void MainWindow::slot_set_selected_half_edge(QListWidgetItem *i) {
    ui->mygl->m_halfEdgeDisplay.destroy();
    ui->mygl->m_halfEdgeDisplay.set_represented_half_edge(static_cast<HalfEdge*>(i));
    ui->mygl->m_halfEdgeDisplay.create();
    ui->mygl->update();
}

void MainWindow::slot_set_selected_face(QListWidgetItem *i) {
    if (ui->mygl->m_faceDisplay.get_represented_face() != nullptr) {
        ui->faceRedSpinBox->setValue(ui->mygl->m_faceDisplay.get_represented_face()->get_color().r);
        ui->faceGreenSpinBox->setValue(ui->mygl->m_faceDisplay.get_represented_face()->get_color().g);
        ui->faceBlueSpinBox->setValue(ui->mygl->m_faceDisplay.get_represented_face()->get_color().b);
    }
    ui->mygl->m_faceDisplay.destroy();
    i = (Face*) i;
    ui->mygl->m_faceDisplay.set_represented_face(static_cast<Face*>(i));
    ui->mygl->m_faceDisplay.create();
    ui->mygl->update();
}

void MainWindow::slot_update_vertex_x_pos(double x) {
    // only set new pos if Vertex is currently selected
    if (ui->mygl->m_vertDisplay.get_represented_vertex() != nullptr) {
        ui->mygl->update_vertex_x_pos(x);
    }
}

void MainWindow::slot_update_vertex_y_pos(double y) {
    // only set new pos if Vertex is currently selected
    if (ui->mygl->m_vertDisplay.get_represented_vertex() != nullptr) {
        ui->mygl->update_vertex_y_pos(y);
    }
}

void MainWindow::slot_update_vertex_z_pos(double z) {
    // only set new pos if Vertex is currently selected
    if (ui->mygl->m_vertDisplay.get_represented_vertex() != nullptr) {
        ui->mygl->update_vertex_z_pos(z);
    }
}

void MainWindow::slot_update_face_r_col(double r) {
    // only set new col if Face is currently selected
    if (ui->mygl->m_faceDisplay.get_represented_face() != nullptr) {
        ui->mygl->update_face_r_col(r);
    }
}

void MainWindow::slot_update_face_g_col(double g) {
    // only set new col if Face is currently selected
    if (ui->mygl->m_faceDisplay.get_represented_face() != nullptr) {
        ui->mygl->update_face_g_col(g);
    }
}

void MainWindow::slot_update_face_b_col(double b) {
    // only set new col if Face is currently selected
    if (ui->mygl->m_faceDisplay.get_represented_face() != nullptr) {
        ui->mygl->update_face_b_col(b);
    }
}

void MainWindow::slot_split_half_edge() {
    // only split HalfEdge if one is currently selected
    if (ui->mygl->m_halfEdgeDisplay.get_represented_half_edge() != nullptr) {
        ui->mygl->split_half_edge();
    }
}

void MainWindow::slot_triangulate_face() {
    // only triangulate Face if one is currently selected
    if (ui->mygl->m_faceDisplay.get_represented_face() != nullptr) {
        // define initial state
        HalfEdge *starting_half_edge = ui->mygl->m_faceDisplay.get_represented_face()->get_half_edge();
        Face *face1_ptr = ui->mygl->m_faceDisplay.get_represented_face();
        HalfEdge *he_0_ptr = face1_ptr->get_half_edge();

        while (he_0_ptr->get_next_half_edge()->get_next_half_edge()->get_next_half_edge() != starting_half_edge) {
            /// TRIANGULATE FUNCTION IN MyGL.cpp:

            // define initial state
            face1_ptr = ui->mygl->m_faceDisplay.get_represented_face();
            he_0_ptr = face1_ptr->get_half_edge();

            // create new Face
            uPtr<Face> face2 = mkU<Face>(face1_ptr->get_color());
            Face *face2_ptr = face2.get();
            ui->mygl->m_mesh.add_face(face2);

            // create two new HalfEdges
            uPtr<HalfEdge> he_a = mkU<HalfEdge>();
            HalfEdge *he_a_ptr = he_a.get();
            ui->mygl->m_mesh.add_half_edge(he_a);
            uPtr<HalfEdge> he_b = mkU<HalfEdge>();
            HalfEdge *he_b_ptr = he_b.get();
            ui->mygl->m_mesh.add_half_edge(he_b);

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
            ui->facesListWidget->addItem(face2_ptr);
            ui->halfEdgesListWidget->addItem(he_a_ptr);
            ui->halfEdgesListWidget->addItem(he_b_ptr);

            ui->mygl->m_faceDisplay.set_represented_face(face2_ptr);

            /// END OF TRIANGULATE FUNCTION IN MyGL.cpp
        }

        // update display
        ui->mygl->m_mesh.destroy();
        ui->mygl->m_mesh.create();
        ui->mygl->m_faceDisplay.destroy();
        ui->mygl->m_faceDisplay.create();
        update();
    }
}

void MainWindow::slot_catmull_clark_subdivision() {
    // save set of original vertices for smoothing operation later on
    std::vector<Vertex*> vertices = ui->mygl->m_mesh.get_vertex_pointers();

    // compute centroids of all Faces
    std::map<Face*, Vertex*> face_centroids = std::map<Face*, Vertex*>();   // maps Face to Vertex at its centroid
    std::vector<Face*> faces = ui->mygl->m_mesh.get_face_pointers();
    for (int i = 0; i < faces.size(); i++) {
        Face *curr_face = faces.at(i);
        int vertex_count = 0;
        HalfEdge *starting_half_edge = curr_face->get_half_edge();
        HalfEdge *curr_half_edge = curr_face->get_half_edge();
        glm::vec3 sum_vertex_pos = curr_half_edge->get_vertex()->get_position();   // add first position
        vertex_count++;
        curr_half_edge = curr_half_edge->get_next_half_edge();
        while (curr_half_edge != starting_half_edge) {
            sum_vertex_pos += curr_half_edge->get_vertex()->get_position();
            vertex_count++;
            curr_half_edge = curr_half_edge->get_next_half_edge();
        }
        uPtr<Vertex> v = mkU<Vertex>(sum_vertex_pos / (float) vertex_count);
        Vertex *v_ptr = v.get();
        ui->mygl->m_mesh.add_vertex(v);
        ui->vertsListWidget->addItem(v_ptr);
        face_centroids.insert(std::pair<Face*, Vertex*>(curr_face, v_ptr));
    }


    // compute smoothed midpoint of each HalfEdge in mesh
    std::set<HalfEdge*> half_edges;
    for(HalfEdge* he : ui->mygl->m_mesh.get_half_edge_pointers()) {
        if(half_edges.find(he->get_parallel_half_edge()) == half_edges.end()) {
                half_edges.insert(he);
        }
    }
    for (HalfEdge* he : half_edges) {
        ui->mygl->split_half_edge_catmull_clark(he, face_centroids);
    }

    // smooth original Vertices
    for (int i = 0; i < vertices.size(); i++) {
        float n = 0;  // n represents number of adjacent midpoints
        Vertex *curr_vertex = vertices.at(i);
        glm::vec3 v = curr_vertex->get_position();    // Vertex's original position

        // find sum of all adjacent midpoints and sum of all adjacent face centroids
        glm::vec3 e = glm::vec3();
        glm::vec3 f = glm::vec3();
        HalfEdge *starting_half_edge = curr_vertex->get_half_edge();
        HalfEdge *curr_half_edge = curr_vertex->get_half_edge();
        e += curr_half_edge->get_parallel_half_edge()->get_vertex()->get_position();
        f += face_centroids[curr_half_edge->get_face()]->get_position();
        n++;
        curr_half_edge = curr_half_edge->get_next_half_edge()->get_parallel_half_edge();
        while (curr_half_edge != starting_half_edge) {
            e += curr_half_edge->get_parallel_half_edge()->get_vertex()->get_position();
            f += face_centroids[curr_half_edge->get_face()]->get_position();
            n++;
            curr_half_edge = curr_half_edge->get_next_half_edge()->get_parallel_half_edge();
        }

        glm::vec3 smoothed_vertex_pos = ((n - 2) * v / n) + (e / (n * n)) + (f / (n * n));
        curr_vertex->set_pos_x(smoothed_vertex_pos[0]);
        curr_vertex->set_pos_y(smoothed_vertex_pos[1]);
        curr_vertex->set_pos_z(smoothed_vertex_pos[2]);
    }

    // quadrangulate each original Face
    for (int i = 0; i < faces.size(); i++) {
        ui->mygl->quadrangulate(faces.at(i), face_centroids[faces.at(i)]);
    }

    // update display
    ui->mygl->m_mesh.destroy();
    ui->mygl->m_mesh.create();
    ui->mygl->m_vertDisplay.destroy();
    ui->mygl->m_vertDisplay.create();
    ui->mygl->m_halfEdgeDisplay.destroy();
    ui->mygl->m_halfEdgeDisplay.create();
    ui->mygl->m_faceDisplay.destroy();
    ui->mygl->m_faceDisplay.create();
    ui->mygl->update();
}

void MainWindow::slot_extrude_face() {
    // only extrude Face if one is currently selected
    Face *selected_face = ui->mygl->m_faceDisplay.get_represented_face();
    if (selected_face != nullptr) {
        bool first = true;
        HalfEdge *starting_half_edge = selected_face->get_half_edge();
        HalfEdge *curr_half_edge = selected_face->get_half_edge();

        Vertex *v3 = nullptr;
        Vertex *v4 = nullptr;
        Vertex *v4_first = nullptr; // save copy of first v4 for last HalfEdge extrusion to avoid repeated Vertex creation
        bool add_v3 = true;
        HalfEdge *he3 = nullptr;
        HalfEdge *he3_prev = nullptr;   // save copy of previous he3 to "sew" HalfEdges together at end
        HalfEdge *he4 = nullptr;
        HalfEdge *he4_first = nullptr;  // save copy of first he4 for last HalfEdge extrusion to "sew" HalfEdges together at end
        Vertex **v3_ptr = &v3;
        Vertex **v4_ptr = &v4;
        HalfEdge **he3_ptr = &he3;
        HalfEdge **he4_ptr = &he4;
        HalfEdge **curr_half_edge_ptr = &curr_half_edge;

        int special_case_num = 0;
        Vertex *v1_first = curr_half_edge->get_vertex();
        Vertex **v1_first_ptr = &v1_first;
        HalfEdge *he1_last = curr_half_edge;
        while (he1_last->get_next_half_edge() != curr_half_edge) {
            he1_last = he1_last->get_next_half_edge();
        }
        HalfEdge **he1_last_ptr = &he1_last;
        Vertex *v2_first = he1_last->get_vertex();
        Vertex **v2_first_ptr = &v2_first;

        while (first || (curr_half_edge != starting_half_edge)) {

            ui->mygl->extrude_half_edge(curr_half_edge_ptr, v3_ptr, v4_ptr, he3_ptr, he4_ptr, special_case_num, v1_first_ptr, v2_first_ptr, he1_last_ptr);

            // add new Face, HalfEdges, and Vertices to GUI list as needed
            if (add_v3) {
                ui->vertsListWidget->addItem(*v3_ptr);
            }
            if (first) {
                ui->vertsListWidget->addItem(*v4_ptr);
            }
            ui->halfEdgesListWidget->addItem(*he3_ptr);
            ui->halfEdgesListWidget->addItem(*he4_ptr);

            // reset for next iteration
            if (first) {
                v4_first = *v4_ptr;
                he4_first = *he4_ptr;
            } else {    // "sew" HalfEdges together
                HalfEdge *he4_temp = *he4_ptr;
                he4_temp->set_parallel_half_edge(&he3_prev);
                he3_prev->set_parallel_half_edge(&he4_temp);
            }
            if (curr_half_edge->get_next_half_edge() == starting_half_edge) {   // last HalfEdge extrusion
                he4_first->set_parallel_half_edge(he3_ptr);
                (*he3_ptr)->set_parallel_half_edge(&he4_first);
            }
            he3_prev = *he3_ptr;

            *v4_ptr = *v3_ptr;
            if (curr_half_edge->get_next_half_edge()->get_next_half_edge()->get_next_half_edge() == starting_half_edge) {
                special_case_num++;
            }
            if (curr_half_edge->get_next_half_edge()->get_next_half_edge() == starting_half_edge) { // about to start last HalfEdge extrusion
                *v3_ptr = v4_first;
                add_v3 = false;
                special_case_num++;
            } else {
                *v3_ptr = nullptr;
                add_v3 = true;
            }
            *he3_ptr = nullptr;
            *he4_ptr = nullptr;
            curr_half_edge = curr_half_edge->get_next_half_edge();
            curr_half_edge_ptr = &curr_half_edge;
            first = false;
        }

        // update display
        ui->mygl->m_mesh.destroy();
        ui->mygl->m_mesh.create();
        ui->mygl->m_faceDisplay.destroy();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::slot_import_obj_file() {
    // get dialogue box to show up
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", "OBJ File (*.obj)");

    ui->mygl->m_mesh.set_is_skinned(false);

    ui->mygl->make_mesh_from_file(filename.toStdString());

    // reset mesh and highlighted Vertex, HalfEdge, and/or Face
    Vertex *v = nullptr;
    ui->mygl->m_vertDisplay.set_represented_vertex(v);
    HalfEdge *he = nullptr;
    ui->mygl->m_halfEdgeDisplay.set_represented_half_edge(he);
    Face *f = nullptr;
    ui->mygl->m_faceDisplay.set_represented_face(f);
    ui->mygl->m_faceDisplay.destroy();
    ui->mygl->m_faceDisplay.create();
    ui->mygl->m_halfEdgeDisplay.destroy();
    ui->mygl->m_halfEdgeDisplay.create();
    ui->mygl->m_vertDisplay.destroy();
    ui->mygl->m_vertDisplay.create();
    Joint *j = nullptr;
    ui->mygl->m_rootJointDisplay.set_selected_joint(j);
    ui->mygl->m_rootJointDisplay.destroy();
    ui->mygl->m_rootJointDisplay.create();
//    ui->mygl->m_mesh.set_is_skinned(false);
    ui->mygl->update();
}

void MainWindow::slot_load_json_file() {
    // get dialogue box to show up
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", "JSON File (*.json)");

    ui->mygl->m_mesh.set_is_skinned(false);

    ui->mygl->read_json(filename.toStdString());

    // reset everything from previous JSON file
    Joint *j = nullptr;
    ui->mygl->m_rootJointDisplay.set_selected_joint(j);
    ui->mygl->m_rootJointDisplay.destroy();
    ui->mygl->m_rootJointDisplay.create();
//    ui->mygl->m_mesh.set_is_skinned(false);
    ui->mygl->update();

    // reset displays
    ui->rotXLabel->setText("0");
    ui->rotYLabel->setText("0");
    ui->rotZLabel->setText("0");
}

void MainWindow::slot_skin_mesh() {
    // only activate skinning function if root joint is set
    if (ui->mygl->m_rootJointDisplay.get_represented_joint() != nullptr) {
        ui->mygl->skin_mesh();
    }
}

void MainWindow::slot_add_joint_to_QTreeWidget(QTreeWidgetItem* j) {
    ui->jointTreeWidget->addTopLevelItem(j);
}

void MainWindow::slot_set_selected_joint(QTreeWidgetItem* j) {
    ui->mygl->m_rootJointDisplay.set_selected_joint((Joint*)(j));
    ui->mygl->m_rootJointDisplay.destroy();
    ui->mygl->m_rootJointDisplay.create();
    ui->mygl->update();

    // update display information
    glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
    glm::vec3 euler = glm::eulerAngles(q);
    ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
    ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
    ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));

    glm::vec3 p = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_pos();
    ui->jointPosXSpinBox->setValue(p[0]);
    ui->jointPosYSpinBox->setValue(p[1]);
    ui->jointPosZSpinBox->setValue(p[2]);
}

void MainWindow::slot_update_joint_x_pos(double x) {
    // only set new pos if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->update_joint_x_pos(x);
    }
}

void MainWindow::slot_update_joint_y_pos(double y) {
    // only set new pos if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->update_joint_y_pos(y);
    }
}

void MainWindow::slot_update_joint_z_pos(double z) {
    // only set new pos if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->update_joint_z_pos(z);
    }
}

void MainWindow::slot_rotate_joint_x_CCW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_x(5.f);  // rotate 5 degrees CCW

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}

void MainWindow::slot_rotate_joint_x_CW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_x(-5.f);  // rotate 5 degrees CW

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}

void MainWindow::slot_rotate_joint_y_CCW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_y(5.f);

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}

void MainWindow::slot_rotate_joint_y_CW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_y(-5.f);  // rotate 5 degrees CW

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}

void MainWindow::slot_rotate_joint_z_CCW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_z(5.f);  // rotate 5 degrees CCW

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}

void MainWindow::slot_rotate_joint_z_CW() {
    // only change rotation if Joint is currently selected
    if (ui->mygl->m_rootJointDisplay.get_selected_joint() != nullptr) {
        ui->mygl->rotate_joint_z(-5.f);  // rotate 5 degrees CW

        // update display information
        glm::quat q = ui->mygl->m_rootJointDisplay.get_selected_joint()->get_rot();
        glm::vec3 euler = glm::eulerAngles(q);
        ui->rotXLabel->setText(QString::number(glm::degrees(euler[0])));
        ui->rotYLabel->setText(QString::number(glm::degrees(euler[1])));
        ui->rotZLabel->setText(QString::number(glm::degrees(euler[2])));
    }
}
