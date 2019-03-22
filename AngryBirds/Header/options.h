#pragma once

#include "utils.h"

class Options
{
public:
	static void optionsMenu(unsigned char key[])
	{
		// Quit.
		if (key[unsigned char('\\')] == 1)
		{
			key[unsigned char('\\')] = 0;
			glutDestroyWindow(0);
			exit(0);
		}
		// Enable Debugging 
		if (key[unsigned char('[')] == 1)
		{
			utils::setDebug();
			system("CLS");
			std::cout << "ASDASDASAS";
			key[unsigned char('[')] = 0;
		}
	}
};