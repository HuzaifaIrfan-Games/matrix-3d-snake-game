#pragma once

#include "Matrix3DSnakeGame/Matrix3DSnakeGame.h"

#include <QObject>
#include <QKeyEvent>

class UserInputController : public QObject
{
    Q_OBJECT
    SnakeDirection currentSnakeDirection=DIR_X_POS;
public:
    explicit UserInputController(QObject *parent = nullptr);

signals:
    void keyPressed(int key);
    void keyReleased(int key);
    void gameResetPressed();
    void gameSnakeDirectionChanged(SnakeDirection snakeDirection); 

public slots:
    void handleKeyPress(Qt::Key key);
    void handleKeyRelease(Qt::Key key);

};
