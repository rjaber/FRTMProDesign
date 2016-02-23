#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mainview.h"

#include <memory>
#include <QMainWindow>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDockWidget>
#include <QSlider>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QExposeEvent>

#include "cameraview.h"
#include "cameras.h"

/**
 * @brief Main window of application
 *
 * This class creates all the GUI widgets that are displayed on the main
 * window. The GUI widgets' signals and slots are also connected in this class.
 */

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /** @brief Constructor*/
    explicit MainWindow(QWidget *parent = 0);

    /** @brief Copying is not allowed*/
    MainWindow(const MainWindow&) = delete;

    /** @brief Copying is not allowed*/
    MainWindow& operator=(const MainWindow&) = delete;

    /** @brief Destructor*/
    ~MainWindow();

public slots:
    /**
     * @brief Adds camera item to list widget
     * @param camID Camera ID.
     * @param camName Camera name.
     */
    void addItemToCameraListWidget(int camID, QString camName);

    /**
     * @brief Deletes camera item in list widget
     * @param camID Camera ID.
     */
    void deleteItemInCameraListWidget(int camID);

    /**
     * @brief Modifies camera's name in list widget
     * @param camID Camera ID.
     * @param camName Camera name.
     */
    void modifyCameraNameInListWidget(int camID, QString camName);


private slots:
    void openObjFile();
    void onBtnCameraDragAndDropPressed();
    void cameraViewWidgetUndocked(bool isUndocked);
    void updateSelectedCameraProperties(int fov, float aspectRatio, QString camName);
    void onlstWidgetCamerasDoubleClicked(const QModelIndex &index);

signals:
    void cameraListDoubleClicked(int);

private:
    /**
     * @brief Creates actions of MainWindow
     */
    void createActions();

    /**
     * @brief Creates menus of MainWindow
     */
    void createMenus();

    /**
     * @brief Creates GUI Widgets and places them in MainWindow
     * @param modelFilePath Full file path of .obj file to load.
     */
    void createGUIWidgets(const QString modelFilePath);

    QHBoxLayout *buttonsLayout_;
    QHBoxLayout *cameraNameLayout_;
    QHBoxLayout *fovLayout_;
    QHBoxLayout *aspectRatioLayout_;
    QVBoxLayout *selectedCameraInfoLayout_;
    QVBoxLayout *leftMainLayout_;
    QHBoxLayout *mainLayout_;

    QAction *newObjFileAction_;
    QAction *exitAction_;
    QMenu *fileMenu_;

    QWidget *leftMainWidget_;
    QWidget *centralWidget_;
    QLabel *lblCamerasList_;
    QListWidget *lstWidgetCameras_;
    QLineEdit *lnEditCameraName_;
    QLabel *lblCameraName_;
    QPushButton *btnUndo_;
    QPushButton *btnDeleteCamera_;
    QPushButton *btnCameraDragAndDrop_;
    QCheckBox *chkBoxCamera180FOV_;
    QSlider *sliderCameraFOVx_;
    QLabel *lblFOVValue_;
    QLabel *lblFOV_;
    QDoubleSpinBox *spnBoxCameraAspectRatio_;
    QLabel *lblAspectRatio_;
    QDockWidget *dockWidgetCameraView_;

    /**
     * @brief cameraListLabels_ Acts as associative array between the list widget's row indices and Camera Ids
     */
    QVector<int> cameraListLabels_;

    MainView *glMainView_;       //Inherits from QObject
    CameraView *glCameraView_;   //Inherits from QObject
    std::shared_ptr<Cameras> cameras_;
};

#endif // MAINWINDOW_H
