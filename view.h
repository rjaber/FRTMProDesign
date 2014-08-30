#ifndef VIEW_H
#define VIEW_H
#include <glm/glm.hpp>

class QMouseEvent;

/**
 * @brief Manages the position and view direction of an OpenGL window's View
 *
 * Provides implementation for travelling in a 3D world via mouse and keyboard inputs.
 *
 * This class is used to:
 *
 * -# Travel in a 3D world with keyboard (WASD) inputs.
 * -# Alter view direction with mouse (left click).
 * -# Zoom in/out with mouse (middle click).
 * -# Transition from one position to another in a 3D world.
 */
class View {
public:
    /**
     * @brief Constructor
     * @param startPos Starting position in 3D world.
     * @param startViewDir Starting Camera view direction.
     * @param up UP vector of View.
     * @param moveSpeed Moving speed while travelling via keyboard input.
     * @param rotationalSpeed Rotating speed while altering view direction orientation via mouse input.
     */
    View(const glm::vec3 &startPos, const glm::vec3 &startViewDir, const glm::vec3 &up,
         const float moveSpeed = 1.0f, const float rotationalSpeed = 0.25f);

    /**
     * @brief Called when mouse button is clicked
     * @param winPosX Window X-Coordinate of mouse position.
     * @param winPosY Window Y-Coordinate of mouse position.
     */
    void mouseDown(const int winPosX, const int winPosY);

    /**
     * @brief Called when mouse is moving
     * @param newMouseEvent New QMouseEvent.
     */
    void mouseUpdate(const QMouseEvent *newMouseEvent);

    /**
     * @brief Sets View to a new position and view direction
     * @param pos New Position.
     * @param vDir New view direction.
     */
    void setView(const glm::vec3& pos, const glm::vec3& vDir) {
        position_ = pos;
        viewDirection_ = vDir;
    }

    /**
     * @brief Gets the View (i.e. lookAt) matrix
     */
    glm::mat4 getViewMatrix() const;

    /**
     * @brief Moves the View forwards
     */
    void moveFoward();

    /**
     * @brief Moves the View backwards
     */
    void moveBackward();

    /**
     * @brief Strafes the View to the left
     */
    void strafeLeft();
    /**
     * @brief Strafes the View to the right
     */
    void strafeRight();

    /**
     * @brief Moves the View up
     */
    void moveUp();

    /**
     * @brief Moves the View down
     */
    void moveDown();

    /**
     * @brief Sets the GoTo position
     * @param gotoPos New GoTo Position.
     */
    void setGoToPosition(const glm::vec3 &gotoPos) {
        gotoPosition_ = gotoPos;
    }

    /**
     * @brief Moves View closer to new GoTo position by a preset increment
     * @return Whether View has reached the new GoTo position.
     */
    bool gotoNewPosition();

    /**
     * @brief Gets View's world position
     */
    glm::vec3 getPosition() const {
        return position_;
    }

    /**
     * @brief Gets View's view direction
     */
    glm::vec3 getViewDirection() const {
        return viewDirection_;
    }

private:
    /**
     * @brief Gets the cross product of the view direction with the UP vector
     */
    glm::vec3 getViewXUP() const;

    glm::vec3 position_;
    glm::vec3 viewDirection_;
    const glm::vec3 UP_;
    glm::vec3 gotoPosition_;
    glm::vec2 oldMousePosition_;
    const float MOVEMENT_SPEED_;
    const float ROTATIONAL_SPEED_;
};

#endif // VIEW_H
