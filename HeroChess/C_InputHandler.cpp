#include "stdafx.h"
#include "C_InputHandler.h"

void C_InputHandler::Handle_Input(C_Hero& hero)
{
	if (A_is_pressed)
		buttonA->execute(hero);
}

void C_InputHandler::Bind_Command()
{
	buttonA = std::make_shared<MoveUnitCommand>();
}

void C_InputHandler::Set_Pressed(char ch)
{
	switch (ch)
	{
	case 'A': A_is_pressed = true; break;
	case 'S': A_is_pressed = false; break;
	}
}
