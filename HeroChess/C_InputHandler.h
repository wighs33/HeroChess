#pragma once
#include "C_Command.h"
#include "C_Hero.h"

class C_InputHandler
{
public:
	void Handle_Input(C_Hero& hero);
	void Bind_Command();
	void Set_Pressed(char ch);
private:
	std::shared_ptr<C_Command> buttonA;
	std::shared_ptr<C_Command> buttonS;

	bool A_is_pressed = false;
};