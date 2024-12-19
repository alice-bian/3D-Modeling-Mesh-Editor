/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    MyGL *mygl;
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *vertPosXSpinBox;
    QDoubleSpinBox *vertPosYSpinBox;
    QDoubleSpinBox *vertPosZSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceBlueSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *faceGreenSpinBox;
    QDoubleSpinBox *faceRedSpinBox;
    QLabel *label_11;
    QPushButton *splitHalfEdgeButton;
    QPushButton *triangulateFaceButton;
    QPushButton *catmullClarkSubdivisionButton;
    QPushButton *extrudeFaceButton;
    QPushButton *importOBJFileButton;
    QPushButton *loadJSONFileButton;
    QPushButton *skinMeshButton;
    QTreeWidget *jointTreeWidget;
    QDoubleSpinBox *jointPosYSpinBox;
    QDoubleSpinBox *jointPosZSpinBox;
    QDoubleSpinBox *jointPosXSpinBox;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QPushButton *jointRotateXCCWButton;
    QPushButton *jointRotateYCCWButton;
    QPushButton *jointRotateZCCWButton;
    QPushButton *jointRotateXCWButton;
    QPushButton *jointRotateYCWButton;
    QPushButton *jointRotateZCWButton;
    QLabel *rotXLabel;
    QLabel *rotYLabel;
    QLabel *rotZLabel;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1321, 492);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QStringLiteral("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QStringLiteral("mygl"));
        mygl->setGeometry(QRect(11, 11, 618, 432));
        vertsListWidget = new QListWidget(centralWidget);
        vertsListWidget->setObjectName(QStringLiteral("vertsListWidget"));
        vertsListWidget->setGeometry(QRect(640, 10, 111, 151));
        halfEdgesListWidget = new QListWidget(centralWidget);
        halfEdgesListWidget->setObjectName(QStringLiteral("halfEdgesListWidget"));
        halfEdgesListWidget->setGeometry(QRect(770, 10, 111, 151));
        facesListWidget = new QListWidget(centralWidget);
        facesListWidget->setObjectName(QStringLiteral("facesListWidget"));
        facesListWidget->setGeometry(QRect(900, 10, 111, 151));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(640, 170, 111, 16));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(770, 170, 111, 16));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(900, 170, 111, 16));
        label_3->setAlignment(Qt::AlignCenter);
        vertPosXSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosXSpinBox->setObjectName(QStringLiteral("vertPosXSpinBox"));
        vertPosXSpinBox->setGeometry(QRect(750, 210, 62, 22));
        vertPosXSpinBox->setMinimum(-100);
        vertPosXSpinBox->setMaximum(100);
        vertPosXSpinBox->setSingleStep(0.1);
        vertPosYSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosYSpinBox->setObjectName(QStringLiteral("vertPosYSpinBox"));
        vertPosYSpinBox->setGeometry(QRect(830, 210, 62, 22));
        vertPosYSpinBox->setMinimum(-100);
        vertPosYSpinBox->setMaximum(100);
        vertPosYSpinBox->setSingleStep(0.1);
        vertPosZSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosZSpinBox->setObjectName(QStringLiteral("vertPosZSpinBox"));
        vertPosZSpinBox->setGeometry(QRect(910, 210, 62, 22));
        vertPosZSpinBox->setMinimum(-100);
        vertPosZSpinBox->setMaximum(100);
        vertPosZSpinBox->setSingleStep(0.1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(650, 210, 91, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(740, 240, 61, 16));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(820, 240, 61, 16));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(900, 240, 61, 16));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(900, 310, 61, 16));
        label_8->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(740, 310, 61, 16));
        label_9->setAlignment(Qt::AlignCenter);
        faceBlueSpinBox = new QDoubleSpinBox(centralWidget);
        faceBlueSpinBox->setObjectName(QStringLiteral("faceBlueSpinBox"));
        faceBlueSpinBox->setGeometry(QRect(910, 280, 62, 22));
        faceBlueSpinBox->setMaximum(255);
        faceBlueSpinBox->setSingleStep(10);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(670, 280, 71, 21));
        faceGreenSpinBox = new QDoubleSpinBox(centralWidget);
        faceGreenSpinBox->setObjectName(QStringLiteral("faceGreenSpinBox"));
        faceGreenSpinBox->setGeometry(QRect(830, 280, 62, 22));
        faceGreenSpinBox->setMaximum(255);
        faceGreenSpinBox->setSingleStep(10);
        faceRedSpinBox = new QDoubleSpinBox(centralWidget);
        faceRedSpinBox->setObjectName(QStringLiteral("faceRedSpinBox"));
        faceRedSpinBox->setGeometry(QRect(750, 280, 62, 22));
        faceRedSpinBox->setMaximum(255);
        faceRedSpinBox->setSingleStep(10);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(820, 310, 61, 16));
        label_11->setAlignment(Qt::AlignCenter);
        splitHalfEdgeButton = new QPushButton(centralWidget);
        splitHalfEdgeButton->setObjectName(QStringLiteral("splitHalfEdgeButton"));
        splitHalfEdgeButton->setGeometry(QRect(640, 340, 181, 32));
        triangulateFaceButton = new QPushButton(centralWidget);
        triangulateFaceButton->setObjectName(QStringLiteral("triangulateFaceButton"));
        triangulateFaceButton->setGeometry(QRect(830, 340, 181, 32));
        catmullClarkSubdivisionButton = new QPushButton(centralWidget);
        catmullClarkSubdivisionButton->setObjectName(QStringLiteral("catmullClarkSubdivisionButton"));
        catmullClarkSubdivisionButton->setGeometry(QRect(640, 370, 181, 32));
        extrudeFaceButton = new QPushButton(centralWidget);
        extrudeFaceButton->setObjectName(QStringLiteral("extrudeFaceButton"));
        extrudeFaceButton->setGeometry(QRect(830, 370, 181, 32));
        importOBJFileButton = new QPushButton(centralWidget);
        importOBJFileButton->setObjectName(QStringLiteral("importOBJFileButton"));
        importOBJFileButton->setGeometry(QRect(1030, 340, 121, 31));
        loadJSONFileButton = new QPushButton(centralWidget);
        loadJSONFileButton->setObjectName(QStringLiteral("loadJSONFileButton"));
        loadJSONFileButton->setGeometry(QRect(1170, 340, 121, 31));
        skinMeshButton = new QPushButton(centralWidget);
        skinMeshButton->setObjectName(QStringLiteral("skinMeshButton"));
        skinMeshButton->setGeometry(QRect(1070, 370, 181, 31));
        jointTreeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        jointTreeWidget->setHeaderItem(__qtreewidgetitem);
        jointTreeWidget->setObjectName(QStringLiteral("jointTreeWidget"));
        jointTreeWidget->setGeometry(QRect(1030, 10, 261, 151));
        jointPosYSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosYSpinBox->setObjectName(QStringLiteral("jointPosYSpinBox"));
        jointPosYSpinBox->setGeometry(QRect(1170, 280, 62, 22));
        jointPosYSpinBox->setMinimum(-100);
        jointPosYSpinBox->setMaximum(100);
        jointPosYSpinBox->setSingleStep(1);
        jointPosZSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosZSpinBox->setObjectName(QStringLiteral("jointPosZSpinBox"));
        jointPosZSpinBox->setGeometry(QRect(1230, 280, 62, 22));
        jointPosZSpinBox->setMinimum(-100);
        jointPosZSpinBox->setMaximum(100);
        jointPosZSpinBox->setSingleStep(1);
        jointPosZSpinBox->setValue(0);
        jointPosXSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosXSpinBox->setObjectName(QStringLiteral("jointPosXSpinBox"));
        jointPosXSpinBox->setGeometry(QRect(1110, 280, 62, 22));
        jointPosXSpinBox->setMinimum(-100);
        jointPosXSpinBox->setMaximum(100);
        jointPosXSpinBox->setSingleStep(1);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(1220, 310, 61, 16));
        label_12->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(1160, 310, 61, 16));
        label_13->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(1100, 310, 61, 16));
        label_14->setAlignment(Qt::AlignCenter);
        jointRotateXCCWButton = new QPushButton(centralWidget);
        jointRotateXCCWButton->setObjectName(QStringLiteral("jointRotateXCCWButton"));
        jointRotateXCCWButton->setGeometry(QRect(1230, 200, 61, 23));
        jointRotateYCCWButton = new QPushButton(centralWidget);
        jointRotateYCCWButton->setObjectName(QStringLiteral("jointRotateYCCWButton"));
        jointRotateYCCWButton->setGeometry(QRect(1230, 220, 61, 23));
        jointRotateZCCWButton = new QPushButton(centralWidget);
        jointRotateZCCWButton->setObjectName(QStringLiteral("jointRotateZCCWButton"));
        jointRotateZCCWButton->setGeometry(QRect(1230, 240, 61, 23));
        jointRotateXCWButton = new QPushButton(centralWidget);
        jointRotateXCWButton->setObjectName(QStringLiteral("jointRotateXCWButton"));
        jointRotateXCWButton->setGeometry(QRect(1110, 200, 61, 23));
        jointRotateYCWButton = new QPushButton(centralWidget);
        jointRotateYCWButton->setObjectName(QStringLiteral("jointRotateYCWButton"));
        jointRotateYCWButton->setGeometry(QRect(1110, 220, 61, 23));
        jointRotateZCWButton = new QPushButton(centralWidget);
        jointRotateZCWButton->setObjectName(QStringLiteral("jointRotateZCWButton"));
        jointRotateZCWButton->setGeometry(QRect(1110, 240, 61, 23));
        rotXLabel = new QLabel(centralWidget);
        rotXLabel->setObjectName(QStringLiteral("rotXLabel"));
        rotXLabel->setGeometry(QRect(1170, 200, 61, 21));
        rotXLabel->setAlignment(Qt::AlignCenter);
        rotYLabel = new QLabel(centralWidget);
        rotYLabel->setObjectName(QStringLiteral("rotYLabel"));
        rotYLabel->setGeometry(QRect(1170, 220, 61, 20));
        rotYLabel->setAlignment(Qt::AlignCenter);
        rotZLabel = new QLabel(centralWidget);
        rotZLabel->setObjectName(QStringLiteral("rotZLabel"));
        rotZLabel->setGeometry(QRect(1170, 240, 61, 20));
        rotZLabel->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(1030, 170, 261, 16));
        label_15->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(1030, 280, 71, 21));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(1030, 200, 71, 21));
        label_17->setAlignment(Qt::AlignCenter);
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(1030, 220, 71, 21));
        label_18->setAlignment(Qt::AlignCenter);
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(1030, 240, 71, 21));
        label_19->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1321, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenGLDemo", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        actionCamera_Controls->setText(QApplication::translate("MainWindow", "Camera Controls", nullptr));
        label->setText(QApplication::translate("MainWindow", "Vertices", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Half-Edges", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Faces", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Vertex Position", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "X", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Y", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Blue", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Red", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Face Color", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Green", nullptr));
        splitHalfEdgeButton->setText(QApplication::translate("MainWindow", "Split HalfEdge", nullptr));
        triangulateFaceButton->setText(QApplication::translate("MainWindow", "Triangulate Face", nullptr));
        catmullClarkSubdivisionButton->setText(QApplication::translate("MainWindow", "Catmull-Clark Subdivision", nullptr));
        extrudeFaceButton->setText(QApplication::translate("MainWindow", "Extrude Face", nullptr));
        importOBJFileButton->setText(QApplication::translate("MainWindow", "Import OBJ File", nullptr));
        loadJSONFileButton->setText(QApplication::translate("MainWindow", "Import JSON File", nullptr));
        skinMeshButton->setText(QApplication::translate("MainWindow", "Bind Mesh", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "Z", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "Y", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "X", nullptr));
        jointRotateXCCWButton->setText(QApplication::translate("MainWindow", "Rotate +X", nullptr));
        jointRotateYCCWButton->setText(QApplication::translate("MainWindow", "Rotate +Y", nullptr));
        jointRotateZCCWButton->setText(QApplication::translate("MainWindow", "Rotate +Z", nullptr));
        jointRotateXCWButton->setText(QApplication::translate("MainWindow", "Rotate- X", nullptr));
        jointRotateYCWButton->setText(QApplication::translate("MainWindow", "Rotate -Y", nullptr));
        jointRotateZCWButton->setText(QApplication::translate("MainWindow", "Rotate -Z", nullptr));
        rotXLabel->setText(QApplication::translate("MainWindow", "--", nullptr));
        rotYLabel->setText(QApplication::translate("MainWindow", "--", nullptr));
        rotZLabel->setText(QApplication::translate("MainWindow", "--", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "Joints", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "Joint Position", nullptr));
        label_17->setText(QApplication::translate("MainWindow", "Rotation", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "Relative to", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "Parent Joint", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
