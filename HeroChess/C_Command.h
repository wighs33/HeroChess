#pragma once
#include "C_Hero.h"

class C_Command
{
public:
    virtual ~C_Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class MoveUnitCommand : public C_Command
{
public:
    MoveUnitCommand(C_Hero* unit, int x, int y)
        :unit_(unit), x_(x), y_(y)
    {
    }
private:
    C_Hero* unit_;
    int x_;
    int y_;
};