#ifndef CAMERASMEMENTO_H
#define CAMERASMEMENTO_H
#include <QMap>

#include "cameras.h"

/**
 * @brief Captures Cameras internal state so that the object can be restored later on
 *
 * The main purpose of this class is to save snapshots of the state of Cameras internal Camera container
 * at a given moment so that it can be restored to that state later to succesfully complete
 * an undo operation.
 */
class CamerasMemento {
public:
    /** @brief Default Constructor*/
    CamerasMemento() = default;

    /** @brief Defualt Copy Constructor*/
    CamerasMemento(const CamerasMemento&) = default;

    /** @brief Defualt Copy Assignment Operator*/
    CamerasMemento& operator=(const CamerasMemento&) = default;

    /** @brief Move Constructor*/
    CamerasMemento(CamerasMemento &&rhs);

    /** @brief Move Assignment operator*/
    CamerasMemento& operator=(CamerasMemento &&rhs);

    /** @brief Destructor*/
    ~CamerasMemento() = default;

    /**
     * @brief Saves the state of Cameras' internal container in a memento instance
     * @param cam Cameras Camera container
     */
    void setState(const QMap<int, Camera> &cam) {
        camerasState_ = cam;
    }

    /**
     * @brief Gets state of Cameras' internal container from a memento instance
     */
    const QMap<int, Camera>& getState() {
       return camerasState_;
    }

private:
    /**
     * @brief Container saves a snapshot of Cameras' internal container
     */
    QMap<int, Camera> camerasState_;
};


#endif // CAMERASMEMENTO_H
