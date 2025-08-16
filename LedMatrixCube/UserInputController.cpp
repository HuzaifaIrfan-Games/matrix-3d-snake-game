// UserInputController.cpp
#include "UserInputController.hpp"

UserInputController::UserInputController(QObject *parent) : QObject(parent)
{
}

void UserInputController::handleKeyPress(Qt::Key key)
{
    // Handle key press
    qDebug() << "Key pressed:" << key;
    emit keyPressed(key);
}

void UserInputController::handleKeyRelease(Qt::Key key)
{
    // Handle key release
    qDebug() << "Key released:" << key;
    emit keyReleased(key);
}