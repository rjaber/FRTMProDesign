#include "camerasmemento.h"

CamerasMemento::CamerasMemento(CamerasMemento &&rhs) {
     camerasState_ = std::move(rhs.camerasState_);
     rhs.camerasState_.clear();
}

CamerasMemento& CamerasMemento::operator=(CamerasMemento &&rhs) {
    if (this != &rhs){
        camerasState_.clear();
        camerasState_ = std::move(rhs.camerasState_);
        rhs.camerasState_.clear();
    }

    return *this;
}
