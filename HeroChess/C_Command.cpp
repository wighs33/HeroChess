#include "stdafx.h"
#include "C_Command.h"

void MoveUnitCommand ::execute(C_Hero* unit)
{
	unit_ = unit;
	if (before_x == -1)
		before_x = unit_->get_x();
	if (before_y == -1)
		before_y = unit_->get_y();
	unit_->Move_Per_Frame(0, 0);
}

void MoveUnitCommand::undo()
{
	if (unit_)
		unit_->Move_Per_Frame(before_x, before_y);
}