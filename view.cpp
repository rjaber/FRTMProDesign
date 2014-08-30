#include "view.h"

#include <QMouseEvent>

#include <glm/gtx/transform.hpp>


View::View(const glm::vec3 &startPos, const glm::vec3 &startViewDir, const glm::vec3 &up,
                    float moveSpeed, float rotationalSpeed) :
                    position_(startPos),
                    viewDirection_(startViewDir),
                    UP_(up),
                    gotoPosition_(startPos),
                    MOVEMENT_SPEED_(moveSpeed),
                    ROTATIONAL_SPEED_(rotationalSpeed) {
}

void View::mouseDown(const int winPosX, const int winPosY) {
    oldMousePosition_ = glm::vec2(winPosX, winPosY);
}

void View::mouseUpdate(const QMouseEvent *newMouseEvent) {
    if (newMouseEvent->buttons() == Qt::MiddleButton)  {  //zoom in or out
        position_ += ((newMouseEvent->y() - oldMousePosition_.y) * MOVEMENT_SPEED_) * viewDirection_;
    }
    else if (newMouseEvent->buttons() == Qt::LeftButton) {  //rotate camera's view direction in place
        glm::mat4 rotatorMatrix = glm::rotate(-(newMouseEvent->x() - oldMousePosition_.x) * ROTATIONAL_SPEED_, UP_) *
                            glm::rotate(-(newMouseEvent->y() - oldMousePosition_.y) * ROTATIONAL_SPEED_, getViewXUP());

        viewDirection_ = glm::mat3(rotatorMatrix) * viewDirection_;
    }

    oldMousePosition_ = glm::vec2(newMouseEvent->x(), newMouseEvent->y());
}

glm::mat4 View::getViewMatrix() const  {
    return glm::lookAt(position_, position_ + viewDirection_, UP_);
}

void View::moveFoward() {
    position_ += MOVEMENT_SPEED_ * viewDirection_;
}

void View::moveBackward() {
    position_ += -MOVEMENT_SPEED_ * viewDirection_;
}

void View::strafeLeft() {
    position_ += -MOVEMENT_SPEED_ * getViewXUP();
}

void View::strafeRight() {
    position_ += MOVEMENT_SPEED_ * getViewXUP();
}

void View::moveUp() {
    position_ += MOVEMENT_SPEED_ * UP_;
}

void View::moveDown() {
    position_ += -MOVEMENT_SPEED_ * UP_;
}

bool View::gotoNewPosition() {
    const static float GOTO_MOVEMENT_SPEED = 0.1f;
    const static float POS_BUFFER = 0.2f;

    if (abs(position_.z - gotoPosition_.z) > POS_BUFFER) {
        position_ += (gotoPosition_ - position_) * GOTO_MOVEMENT_SPEED;
        return true;
    }
    else {
        return false;
    }
}

glm::vec3 View::getViewXUP() const {
    return glm::cross(viewDirection_, UP_);
}
