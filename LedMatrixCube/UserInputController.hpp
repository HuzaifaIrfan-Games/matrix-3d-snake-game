#pragma once


#include <QObject>
#include <QKeyEvent>

class UserInputController : public QObject
{
    Q_OBJECT
public:
    explicit UserInputController(QObject *parent = nullptr);

signals:
    void keyPressed(int key);
    void keyReleased(int key);

public slots:
    void handleKeyPress(Qt::Key key);
    void handleKeyRelease(Qt::Key key);
};
