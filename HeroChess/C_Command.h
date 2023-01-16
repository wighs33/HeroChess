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
    virtual void execute(C_Hero* unit) override
	{
		unit_ = unit;
		if (before_x == -1)
			before_x = unit_->get_x();
		if (before_y == -1)
			before_y = unit_->get_y();
		unit_->Move_Per_Frame(0, 0);
	}

	virtual void undo() override
	{
		if (unit_)
			unit_->Move_Per_Frame(before_x, before_y);
	}
private:
	C_Hero* unit_;
	int x_, y_;
	int before_x = -1, before_y = -1;
};