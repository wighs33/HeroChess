#pragma once
#include "C_Hero.h"

class C_Command
{
public:
    virtual ~C_Command() {}
    virtual void execute(C_Hero* unit) = 0;
    virtual void undo() = 0;
};

class MoveUnitCommand : public C_Command
{
public:
	virtual void execute(C_Hero* unit) override;
	virtual void undo() override;
private:
	C_Hero* unit_;
	int x_, y_;
	int before_x = -1, before_y = -1;
};