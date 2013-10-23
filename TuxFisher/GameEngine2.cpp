#include "GameEngine2.h"

GameEngine2::GameEngine2(QObject *parent) :
    QObject(parent)
{
}

Square::Type moveTux(Move::Type move)
{
    return Square::Void;
}
