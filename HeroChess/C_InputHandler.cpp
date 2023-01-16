#include "stdafx.h"
#include "C_InputHandler.h"

void C_InputHandler::Handle_Input(C_Hero* hero)
{
	if (A_is_pressed)
		buttonAZ->execute(hero);
	else if (Z_is_pressed)
		buttonAZ->undo();
}

void C_InputHandler::Bind_Command()
{
	buttonAZ = std::make_shared<MoveUnitCommand>();
}

void C_InputHandler::Set_Pressed(char ch)
{
	switch (ch)
	{
	case 'A': 
	{
		A_is_pressed = true;
		Z_is_pressed = false;
		break;
	}
	case 'S': 
	{
		A_is_pressed = false; 
		Z_is_pressed = false;
		break;
	}
	case 'Z': {
		A_is_pressed = false;
		Z_is_pressed = true; 
		break;
	}
	}
}
