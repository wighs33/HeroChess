#pragma once
#include "C_Hero.h"

class C_Command
{
public:
    virtual ~C_Command() {}
    virtual void execute(C_Hero& unit) = 0;
    virtual void undo() = 0;
};

class MoveUnitCommand : public C_Command
{
public:
    virtual void execute(C_Hero& unit)
    {
        unit.Move_Per_Frame();
    }

    virtual void undo()
    {

    }
};