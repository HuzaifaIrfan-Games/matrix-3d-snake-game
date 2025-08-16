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
        emit gameSnakeDirectionChanged(DIR_Y_POS);
        break;
    case Qt::Key_K:
        emit gameSnakeDirectionChanged(DIR_Y_NEG);
        break;
    case Qt::Key_J:
        emit gameSnakeDirectionChanged(DIR_X_NEG);
        break;
    case Qt::Key_L:;
        emit gameSnakeDirectionChanged(DIR_X_POS);
        break;
    case Qt::Key_Delete:
        emit gameSnakeDirectionChanged(DIR_Z_POS);
        break;
    case Qt::Key_Insert:
        emit gameSnakeDirectionChanged(DIR_Z_NEG);
        break;

    case Qt::Key_Home:
        emit gameResetPressed();
        break;
    case Qt::Key_End:
        emit gameStopPressed();
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