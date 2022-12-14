#include <random>
#include <vector>
#include <iostream>
#include <sstream>
#include "NumberFinder.h"
#include <thread>
#include "Round.h"
#include "MathInputReader.h"
#include <map>

#pragma once

enum NumSelection
{
  big,
  small,
};

class MathRound : public Round
{
  std::mt19937 randomGen;
  std::uniform_int_distribution<> targetDistribution;

  std::vector<int> bigNums = {100, 75, 50, 25};
  std::vector<int> smallNums = {
    1, 1,
    2, 2,
    3, 3,
    4, 4,
    5, 5,
    6, 6,
    7, 7, 
    8, 8,
    9, 9,
    10, 10,
  };
  int target = 0;
  int playerAnswer = INT_MIN;
  std::vector<int> gameNums = {};
  NumberFinder* finder = nullptr;
  bool finderHasFinished = false;
  MathRoundAnswer* answer = nullptr;
  MathRoundInputReader reader;

  void addSmallNum()
  {
    auto it = smallNums.begin();
    std::uniform_int_distribution<> randIndex(0, (smallNums.size() - 1));
    int index = randIndex(randomGen);
    it += index;
    this->gameNums.push_back(smallNums[index]);
    smallNums.erase(it);
  }

  void addBigNum()
  {
    if (bigNums.size() == 0)
    {
      addSmallNum();
    }
    else
    {
      auto it = bigNums.begin();
      std::uniform_int_distribution<> randIndex(0, (bigNums.size() - 1));
      int index = randIndex(randomGen);
      it += index;
      this->gameNums.push_back(bigNums[index]);
      bigNums.erase(it);
    }
  }

  void getRandomTarget()
  {
      while (this->target == 0)
      {
          int potentialTarget = this->targetDistribution(randomGen);
          // Rerolls if potential target is 100 to avoid the fringe situation where a player could also get a 100 and win immediately.
          if (potentialTarget == 100 && std::any_of(this->gameNums.begin(), this->gameNums.end(), [](int val) { return val == 100; })) continue;
          this->target = potentialTarget;
      }
      
    
  }

  void reinitializeNumChoices()
  {
    this->bigNums = {100, 75, 50, 25};
    this->smallNums = {
      1, 1,
      2, 2,
      3, 3,
      4, 4,
      5, 5,
      6, 6,
      7, 7, 
      8, 8,
      9, 9,
      10, 10,
    };
  }

  bool answerFound()
  {
    return this->finderHasFinished;
  }

  void awaitAnswer()
  {
    int loopBreaker = 0;
    while(!this->finderHasFinished && loopBreaker < 120)
    { 
      std::this_thread::sleep_for(std::chrono::seconds(1));
      loopBreaker++;
      if (loopBreaker % 5 == 0 && loopBreaker != 0)
      {
        std::cout << "Have waited for " << std::to_string(loopBreaker) << " seconds..." << std::endl; 
      }
      if (loopBreaker >= 60 && loopBreaker % 10 == 0)
      {
        std::cout << "This has taken me a really long time. If you like, you can press ctrl+c to cancel this program." << std::endl;
      }
    }
    if (!this->finderHasFinished) throw std::runtime_error("The answer could not be found in two minutes");
  }

  void getPlayerAnswer()
  {
      this->playerAnswer = this->reader.startUp(this->target, this->gameNums);
      this->respondToPlayerAnswer(this->playerAnswer);
  }

public:
  MathRound()
  {
    std::random_device gen;
    this->randomGen = std::mt19937(gen());
    this->targetDistribution = std::uniform_int_distribution<>(100, 999);
  }

  void addNum(NumSelection selection)
  {
    if (selection == big)
    {
      this->addBigNum();
    }
    if (selection == small)
    {
      this->addSmallNum();
    }
  }

/**
 * @brief Gives player feedback according to the four possible outcomes of the round.
 * 
 * 1. Player got the exact answer.
 * 2. Player did not get the exact answer and it was possible.
 * 3. Answer was not possible but the player got the closest to it possible.
 * 4. Answer was not possible and the player did not get as close as possible. 
 * 
 * @param playerAnswer 
 */
  void respondToPlayerAnswer(const int& playerAnswer)
  {
    if (playerAnswer == this->target)
    {
      std::cout << "That's it. Good job, 10 points." << std::endl;
      return;
    }

    if (!this->finderHasFinished)
    {
      std::cout << "This was a hard one. I need a bit to think about it." << std::endl;
      try
      {
        this->awaitAnswer();
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        //TODO: Implement some form of error handling for a very slow response from awaitAnswer()
        return;
      }
    }

    if (this->answer->isPossible()) this->answer->printAnswer();

    else if (abs(this->target - playerAnswer) == this->answer->getClosestDistance())
    {
      std::cout << "Your answer is the closest one can get to " << std::to_string(this->target) << " with the given numbers." << std::endl;
    }

    else this->answer->printAnswer();

    if (playerAnswer == INT_MIN) std::cout << "You did not provide an answer and thus get no points." << std::endl;
    else std::cout << "\nYour answer was " << std::to_string(playerAnswer) << std::endl;
  }

  //TODO: Setup Round. Transfer to inputReader. Get numbers to use.

  void setupNumbers()
  {
    int numOfBigs = this->reader.askForNumbers();

    for (int i = 0; i < 6; i++)
    {
        if (i < numOfBigs) this->addNum(big);
        else this->addNum(small);
    }
  }

/**
 * @brief 
 * @throws Throws if the gameNums vector doesn't have 6 numbers.
 */
  void startRound()
  {
      this->setupNumbers();
    if (this->gameNums.size() != 6) throw std::runtime_error("Not enough numbers");
    this->getRandomTarget();
    this->finder = new NumberFinder(this->target, this->gameNums);
    // Separate thread to allow for calculating answer while querying player for their answer.
    std::thread finding([this] { this->answer = (*finder).findAnswer(); this->finderHasFinished = true; });
    finding.detach();

    this->getPlayerAnswer();
  }

  void cleanupRound()
  {
    this->reinitializeNumChoices();
    this->gameNums.clear();
    this->finderHasFinished = false;
    this->playerAnswer = INT_MIN;
    this->target = 0;
    delete answer;
    delete finder;
  }

  int getPlayerScore() 
  {
      // Getting best distance from the answer object to ensure accurate scoring when target is not able to be reached.
      int bestAnswerDistance = this->answer->getClosestDistance();
      // If score is guaranteed to be more than 10 points away from any target, just return 0.
      if (this->playerAnswer < 90 || this->playerAnswer > 1009) return 0;
      const int playersDistance = abs(this->target - this->playerAnswer);

      const int distance = abs(bestAnswerDistance - playersDistance);
      if (distance == 0) return 10;
      else if (distance >= 1 && distance <= 5) return 7;
      else if (distance >= 5 && distance <= 10) return 5;
      else return 0;
  }
};