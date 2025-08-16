// UserInputController.cpp
#include "UserInputController.hpp"



UserInputController::UserInputController(QObject *parent) : QObject(parent)
{
}

void UserInputController::handleKeyPress(Qt::Key key)
{
    // Handle key press
    // qDebug() << "Key pressed:" << key;
    emit keyPressed(key);


    // Update the snake direction based on the key pressed
    switch (key) {
    case Qt::Key_I:
        if (currentSnakeDirection != DIR_Y_NEG) {
            currentSnakeDirection = DIR_Y_POS;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;
    case Qt::Key_K:
        if (currentSnakeDirection != DIR_Y_POS) {
            currentSnakeDirection = DIR_Y_NEG;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;
    case Qt::Key_J:
        if (currentSnakeDirection != DIR_X_POS) {
            currentSnakeDirection = DIR_X_NEG;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;
    case Qt::Key_L:
        if (currentSnakeDirection != DIR_X_NEG) {
            currentSnakeDirection = DIR_X_POS;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;
    case Qt::Key_Insert:
        if (currentSnakeDirection != DIR_Z_POS) {
            currentSnakeDirection = DIR_Z_NEG;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;
    case Qt::Key_Delete:
        if (currentSnakeDirection != DIR_Z_NEG) {
            currentSnakeDirection = DIR_Z_POS;
            emit gameSnakeDirectionChanged(currentSnakeDirection);
        }
        break;

    case Qt::Key_Enter:
        emit gameResetPressed();
        break;
    }
    // qDebug() << "Current snake direction:" << currentSnakeDirection;
}

void UserInputController::handleKeyRelease(Qt::Key key)
{
    // Handle key release
    // qDebug() << "Key released:" << key;
    emit keyReleased(key);
}