#include "frtmutils.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QString>
#include <QFile>
#include <QFileDevice>
#include <QDebug>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


QString FRTMUtils::getModelFilePath(const QString &objFilePath) {
    QStringList modelFilePathStringList = objFilePath.split("/", QString::SkipEmptyParts);
    modelFilePathStringList.removeLast();

    QString modelFilePath = modelFilePathStringList.join("/");
    modelFilePath.append("/");
    return modelFilePath;
}


/**
 * @brief Reads file error info if a .obj or .mtl text file failed to load successfully
 * @param file .obj or .mtl QFile.
 */
void FRTMUtils::readFileErrorInfo(const QFile &file) {
        QString errMsg;
        errMsg = file.errorString();

        QFileDevice::FileError errStatus = QFileDevice::NoError;
        errStatus = file.error();

        qDebug () <<"Error Opening File" << "\nError Message: " << errMsg << "\nError Status:" << errStatus ;
}


/**
 * @brief Sets minimum dimensions of window based on monitor's dimensions and minimum width/height factors
 */
void FRTMUtils::setWidgetMinimumSize(QWidget *widget, float minWidthFactor, float minHeightFactor) {

    const QRect SCREEN_GEOMETRY = QApplication::desktop()->screenGeometry();

    const int MAINVIEW_MIN_WIDTH = SCREEN_GEOMETRY.width() * minWidthFactor;
    const int MAINVIEW_MIN_HEIGHT = SCREEN_GEOMETRY.height() * minHeightFactor;

    widget->setMinimumSize(MAINVIEW_MIN_WIDTH, MAINVIEW_MIN_HEIGHT);
}


