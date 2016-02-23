#include "mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QGLFormat>
#include <QtAlgorithms>
#include <QApplication>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle("3D Surveillance Designer");
    this->showMaximized();
    createActions();
    createMenus();
}

MainWindow::~MainWindow() {
}

void MainWindow::addItemToCameraListWidget(int camID, QString camName) {
    cameraListLabels_.push_back(camID);
    //Keep container sorted based on camIDs
    qSort(cameraListLabels_);

    lstWidgetCameras_->insertItem(cameraListLabels_.indexOf(camID), camName);
}

void MainWindow::deleteItemInCameraListWidget(int camID) {
    lstWidgetCameras_->takeItem(cameraListLabels_.indexOf(camID));
    cameraListLabels_.remove(cameraListLabels_.indexOf(camID));
}

void MainWindow::modifyCameraNameInListWidget(int camID, QString camName) {
    lstWidgetCameras_->item(cameraListLabels_.indexOf(camID))->setText(camName);
}

void MainWindow::openObjFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File",
                                                     "", "Files (*.obj)");
    if (filePath != "")
        createGUIWidgets(filePath);
}

void MainWindow::onBtnCameraDragAndDropPressed() {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/images/camera.png", 0, Qt::AutoColor));

    drag->setHotSpot(QPoint(5, 15));
    drag->exec(Qt::CopyAction);
}

void MainWindow::cameraViewWidgetUndocked(bool isUndocked) {

    const QRect SCREEN_GEOMETRY = QApplication::desktop()->screenGeometry();

    const float XPOS_WIDTH_FACTOR = 0.20f;
    const float YPOS_HEIGHT_FACTOR = 0.5f;
    const int UNDOCKED_CAMVIEW_XPOS = SCREEN_GEOMETRY.width() * XPOS_WIDTH_FACTOR;
    const int UNDOCKED_CAMVIEW_YPOS = SCREEN_GEOMETRY.height() * YPOS_HEIGHT_FACTOR;

    if (isUndocked)
        dockWidgetCameraView_->move(UNDOCKED_CAMVIEW_XPOS, UNDOCKED_CAMVIEW_YPOS);
}

void MainWindow::updateSelectedCameraProperties(int fov, float aspectRatio, QString camName) {
    if (lstWidgetCameras_->count()) {
        sliderCameraFOVx_->setEnabled(true);
        spnBoxCameraAspectRatio_->setEnabled(true);
        lnEditCameraName_->setEnabled(true);
        chkBoxCamera180FOV_->setEnabled(true);
    } else {
        sliderCameraFOVx_->setEnabled(false);
        spnBoxCameraAspectRatio_->setEnabled(false);
        lnEditCameraName_->setEnabled(false);
        chkBoxCamera180FOV_->setEnabled(false);
    }

    sliderCameraFOVx_->setValue(fov);
    spnBoxCameraAspectRatio_->setValue(aspectRatio);
    lnEditCameraName_->setText(camName);
}

void MainWindow::onlstWidgetCamerasDoubleClicked(const QModelIndex &index) {
    emit cameraListDoubleClicked(cameraListLabels_[index.row()]);
}

void MainWindow::createActions() {
    newObjFileAction_ = new QAction("Load OBJ Model", this);
    newObjFileAction_->setShortcut(QKeySequence::New);
    newObjFileAction_->setIcon(QIcon(":/images/obj.png"));
    newObjFileAction_->setStatusTip("Loads OBJ Model into the Main View");
    connect(newObjFileAction_, SIGNAL(triggered()), this, SLOT(openObjFile()));

    exitAction_ = new QAction("Exit", this);
    exitAction_->setShortcut(QKeySequence::Quit);
    exitAction_->setStatusTip("Exit the application");
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus() {
    fileMenu_  = this->menuBar()->addMenu("&File");
    fileMenu_->addAction(newObjFileAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);
}

void MainWindow::createGUIWidgets(const QString modelFilePath ) {

    cameras_ = Cameras::getInstance();

    connect(cameras_.get(), SIGNAL(cameraAdded(int, QString)),
            this, SLOT(addItemToCameraListWidget(int, QString)));

    connect(cameras_.get(), SIGNAL(cameraNameModified(int, QString)),
            this, SLOT(modifyCameraNameInListWidget(int, QString)));

    connect(cameras_.get(), SIGNAL(cameraDeleted(int)),
            this, SLOT(deleteItemInCameraListWidget(int)));

    connect(cameras_.get(), SIGNAL(setSelectedCameraPropertiesInMainWindow(int, float, QString)),
            this, SLOT(updateSelectedCameraProperties(int, float, QString)));

    connect(this, SIGNAL(cameraListDoubleClicked(int)),
            cameras_.get(), SLOT(setSelectedCamera(int)));

    //ListWidget
    lblCamerasList_ = new QLabel("Cameras");
    lstWidgetCameras_ = new QListWidget();
    connect(lstWidgetCameras_, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(onlstWidgetCamerasDoubleClicked(QModelIndex)));

    //Selected Camera Info Widgets
    lblCameraName_ = new QLabel("Name");
    lnEditCameraName_ = new QLineEdit();
    lblCameraName_->setBuddy(lnEditCameraName_);
    lnEditCameraName_->setEnabled(false);
    connect(lnEditCameraName_, SIGNAL(textEdited(QString)),
            cameras_.get(), SLOT(setSelectedCameraName(QString)));

    cameraNameLayout_ = new QHBoxLayout();
    cameraNameLayout_->addWidget(lblCameraName_);
    cameraNameLayout_->addWidget(lnEditCameraName_);

    lblFOV_ = new QLabel("FOVx");
    lblFOVValue_ = new QLabel("0");
    sliderCameraFOVx_ = new QSlider(Qt::Horizontal);
    sliderCameraFOVx_->setValue(0);
    sliderCameraFOVx_->setEnabled(false);
    connect(sliderCameraFOVx_, SIGNAL(valueChanged(int)),
            lblFOVValue_, SLOT(setNum(int)));

    connect(sliderCameraFOVx_, SIGNAL(sliderMoved(int)),
            cameras_.get(), SLOT(setSelectedCameraFOV(int)));

    connect(sliderCameraFOVx_, SIGNAL(sliderPressed()),
            cameras_.get(), SLOT(setMemento()));

    fovLayout_ = new QHBoxLayout();
    fovLayout_->addWidget(lblFOV_);
    fovLayout_->addWidget(lblFOVValue_);
    fovLayout_->addWidget(sliderCameraFOVx_);

    lblAspectRatio_ = new QLabel("Aspect Ratio");
    spnBoxCameraAspectRatio_ = new QDoubleSpinBox();
    spnBoxCameraAspectRatio_->setEnabled(false);
    spnBoxCameraAspectRatio_->setSingleStep(0.1);
    connect(spnBoxCameraAspectRatio_, SIGNAL(valueChanged(double)),
            cameras_.get(), SLOT(setSelectedCameraAspectRatio(double)));

    aspectRatioLayout_ = new QHBoxLayout();
    aspectRatioLayout_->addWidget(lblAspectRatio_);
    aspectRatioLayout_->addWidget(spnBoxCameraAspectRatio_);

    selectedCameraInfoLayout_ = new QVBoxLayout();
    selectedCameraInfoLayout_->addLayout(cameraNameLayout_);
    selectedCameraInfoLayout_->addLayout(fovLayout_);
    selectedCameraInfoLayout_->addLayout(aspectRatioLayout_);

    //Buttons Widgets
    btnUndo_ = new QPushButton();
    btnUndo_->resize(64, 64);
    btnUndo_->setIcon(QPixmap(":/images/undo.png", 0, Qt::AutoColor));
    btnUndo_->setIconSize(QSize(32, 32));
    connect(btnUndo_, SIGNAL(clicked()),
            cameras_.get(), SLOT(getMemento()));

    btnDeleteCamera_ = new QPushButton();
    btnDeleteCamera_->resize(64, 64);
    btnDeleteCamera_->setIcon(QPixmap(":/images/delete.png", 0, Qt::AutoColor));
    btnDeleteCamera_->setIconSize(QSize(32, 32));
    connect(btnDeleteCamera_, SIGNAL(pressed()),
            cameras_.get(), SLOT(deleteSelectedCamera()));

    btnCameraDragAndDrop_ = new QPushButton();
    btnCameraDragAndDrop_->resize(64, 64);
    btnCameraDragAndDrop_->setIcon(QPixmap(":/images/camera.png", 0, Qt::AutoColor));
    btnCameraDragAndDrop_->setIconSize(QSize(32, 32));
    connect(btnCameraDragAndDrop_, SIGNAL(pressed()),
            this, SLOT(onBtnCameraDragAndDropPressed()));

    buttonsLayout_ = new QHBoxLayout();
    buttonsLayout_->addStretch();
    buttonsLayout_->addWidget(btnDeleteCamera_);
    buttonsLayout_->addWidget(btnUndo_);
    buttonsLayout_->addWidget(btnCameraDragAndDrop_);

    //Checkbox Widget
    chkBoxCamera180FOV_ = new QCheckBox("Show Full Coverage");
    chkBoxCamera180FOV_->setChecked(false);

    //MainWindow OpenGL Widgets
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CoreProfile);
    format.setSampleBuffers(true);
    format.setSamples(4);

    glMainView_ = new MainView(format, modelFilePath);
    glCameraView_ = new CameraView(glMainView_);

    dockWidgetCameraView_ = new QDockWidget();
    dockWidgetCameraView_->setAllowedAreas(Qt::NoDockWidgetArea);
    dockWidgetCameraView_->setFeatures(QDockWidget::DockWidgetMovable |
                                       QDockWidget::DockWidgetFloatable);
    dockWidgetCameraView_->setWidget(glCameraView_);
    dockWidgetCameraView_->setWindowTitle("Selected Camera View");
    connect(dockWidgetCameraView_, SIGNAL(topLevelChanged(bool)),
            this, SLOT(cameraViewWidgetUndocked(bool)));

    //Left Layout
    leftMainWidget_ = new QWidget();
    leftMainLayout_ = new QVBoxLayout(leftMainWidget_);
    leftMainLayout_->addWidget(lblCamerasList_);
    leftMainLayout_->addWidget(lstWidgetCameras_);
    leftMainLayout_->addLayout(buttonsLayout_);
    leftMainLayout_->addLayout(selectedCameraInfoLayout_);
    leftMainLayout_->addWidget(chkBoxCamera180FOV_);
    leftMainLayout_->addStretch(10);
    leftMainLayout_->addWidget(dockWidgetCameraView_);
    const float LEFT_MAIN_LAYOUT_WIDTH_FACTOR = 1 / 3.75f;
    leftMainWidget_->setFixedWidth(this->width() * LEFT_MAIN_LAYOUT_WIDTH_FACTOR);

    mainLayout_ = new QHBoxLayout();
    mainLayout_->addWidget(leftMainWidget_);
    mainLayout_->addWidget(glMainView_);

    centralWidget_ = new QWidget();
    centralWidget_->setLayout(mainLayout_);

    this->setCentralWidget(centralWidget_); //now all QObjects created using "new" in this class are owned by MainWindow

    connect(chkBoxCamera180FOV_, SIGNAL(toggled(bool)),
            glMainView_, SLOT(toggelShowCamera180FOVView(bool)));
}
