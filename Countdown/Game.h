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
	MathRound* mathRound;
	LettersRound* lettersRound;
	int numberOfRoundsEach = 2;

	void switchRound()
	{
		currentRound == mathRound ? currentRound = lettersRound : currentRound = mathRound;
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

public:
	Game()
	{
		this->mathRound = new MathRound();
		this->lettersRound = new LettersRound();
		this->currentRound = this->lettersRound;
	};

	~Game()
	{
		this->currentRound = nullptr;
		delete this->mathRound;
		delete this->lettersRound;
	}

	void playGame()
	{
		int roundsComplete = 0;

		for (roundsComplete; roundsComplete < this->numberOfRoundsEach * 2; roundsComplete++)
		{
			this->playCurrentRound();
			this->switchRound();
		}

		this->printScore();
	}
};