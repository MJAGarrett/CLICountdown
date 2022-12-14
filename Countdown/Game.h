#include "MathRound.h"
#include "LettersRound.h"

#pragma once

enum RoundName
{
	math,
	letters,
};

class Game
{
	int score = 0;
	Round* currentRound;
	MathRound mathRound;
	LettersRound lettersRound;
	int numberOfRoundsEach = 2;

	void switchRound()
	{
		currentRound == &mathRound ? currentRound = &lettersRound : currentRound = &mathRound;
	}

	void printScore()
	{
		std::cout << "You have earned " << std::to_string(score) << " points." << std::endl;
	}

	void playCurrentRound()
	{
		this->currentRound->startRound();
		int roundScore = this->currentRound->getPlayerScore();
		this->currentRound->cleanupRound();
		this->score += roundScore;
	}

	void conundrum()
	{
		std::cout << "Conundrum awaiting implementation." << std::endl;
	}

public:
	Game()
	{
		this->mathRound = MathRound();
		this->lettersRound = LettersRound();
		this->currentRound = &(this->lettersRound);
	};

	void playGame()
	{
		int roundsComplete = 0;

		for (roundsComplete; roundsComplete < this->numberOfRoundsEach * 2; roundsComplete++)
		{
			this->playCurrentRound();
			this->switchRound();
		}

		this->conundrum();

		this->printScore();
	}
};