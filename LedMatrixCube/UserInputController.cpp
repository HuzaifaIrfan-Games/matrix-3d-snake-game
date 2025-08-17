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
    switch (key)
    {
    case Qt::Key_5:
        emit gameSnakeDirectionChanged(DIR_Z_NEG);
        break;
    case Qt::Key_2:
        emit gameSnakeDirectionChanged(DIR_Z_POS);
        break;

    case Qt::Key_1:
        emit gameSnakeDirectionChanged(DIR_X_NEG);
        break;
    case Qt::Key_3:
        emit gameSnakeDirectionChanged(DIR_X_POS);
        break;

    case Qt::Key_7:
        emit gameSnakeDirectionChanged(DIR_Y_POS);
        break;
    case Qt::Key_4:
        emit gameSnakeDirectionChanged(DIR_Y_NEG);
        break;

    case Qt::Key_Enter:
        emit gameResetPressed();
        break;
    case Qt::Key_Period:
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