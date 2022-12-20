#include <iostream>
#include "Round.h"
#include "LettersInputReader.h"
#include "Dictionary.h"

#pragma once

class LettersRound : public Round
{
	LettersInputReader reader;
	Dictionary dictionary;

public:
	void startRound()
	{
		std::cout << "Awaiting implementation" << std::endl;
	}
	void startCondundrum()
	{
		std::cout << "Conundrum awaiting implementation" << std::endl;
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