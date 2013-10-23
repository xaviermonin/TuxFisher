#ifndef GAMEENGINE2_H
#define GAMEENGINE2_H

#include <QObject>
#include <QRect>

#include "Map.h"

namespace Move {
    enum Type { Up, Down, Left, Right };
}

class GameEngine2 : public QObject
{
Q_OBJECT
public:
    explicit GameEngine2(QObject *parent = 0);

    Square::Type moveTux(Move::Type move);

signals:
    void updateWidget(const Map&, QRect);
public slots:

};

#endif // GAMEENGINE2_H
