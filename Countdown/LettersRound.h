#include <iostream>
#include "Round.h"

#pragma once

class LettersRound : public Round
{
public:
	void startRound()
	{
		std::cout << "Awaiting implementation" << std::endl;
	}
	int getPlayerScore()
	{
		return 0;
	}
	void cleanupRound()
	{
		return;
	}
};