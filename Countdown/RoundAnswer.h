#include <string>
#include <vector>
#include <iostream>
#include "ClosestNumContext.h"

#pragma once

class RoundAnswer
{
public:
  virtual void printAnswer() const = 0;
};

class MathRoundAnswer : RoundAnswer
{
  const bool targetReachable;
  const int target;
  std::pair<ClosestNumContext, ClosestNumContext> closestNums;
  // closerOfTheTwo should remain a nullptr if both closest nums are the same distance away from the target.
  ClosestNumContext* closerOfTheTwo = nullptr;
  int closestDistanceToTarget;

  void printHistory(const std::vector<std::string> &history) const
  {
    size_t size = history.size();

    std::cout << "\tFirst  : " << history[0] << std::endl;
    if (size == 1) return;

    for (int i = 1; i < history.size() - 1; i++)
    {
      std::cout << "\tThen   : " << history[i] << std::endl;
    }

    std::cout << "\tFinally: " << history[history.size() - 1] << std::endl;
  }

  std::string getClosestNumsString() const
  {
      if (closerOfTheTwo == nullptr)
      {
          return std::to_string(this->closestNums.first.num) + " or " + std::to_string(this->closestNums.second.num);
      }
      else
      {
          return std::to_string(this->closerOfTheTwo->num);
      }
  }
public:
  MathRoundAnswer(int target, std::pair<ClosestNumContext, ClosestNumContext> closestNums) 
      : target(target), closestNums(closestNums), targetReachable(target == closestNums.first.num || target == closestNums.second.num) 
  {
      const int lowerDistance = abs(this->target - this->closestNums.first.num);
      const int upperDistance = abs(this->target - this->closestNums.second.num);

      if (lowerDistance == upperDistance) 
      {
          this->closestDistanceToTarget = lowerDistance;
      }
      else if (lowerDistance > upperDistance) 
      {
          this->closerOfTheTwo = &this->closestNums.second;
          this->closestDistanceToTarget = upperDistance;
      }
      else 
      {
          this->closerOfTheTwo = &this->closestNums.first;
          this->closestDistanceToTarget = lowerDistance;
      }
  }

  void printAnswer() const
  {
    if (!targetReachable)
    {
      std::cout << "It is not possible to get " << std::to_string(this->target) << " with the given numbers." << std::endl;
      std::cout << "The closest one can get to " << std::to_string(this->target) << " is " << this->getClosestNumsString() << "." << std::endl;
      if (this->closerOfTheTwo != nullptr)
      {
          std::cout << "One way to get to " << std::to_string(this->closerOfTheTwo->num) + " is:" << std::endl;
          this->printHistory(this->closerOfTheTwo->stepsToNum);
      }
      else
      {
          std::cout << "One way to get to " << std::to_string(this->closestNums.first.num) << ", for example, is: " << std::endl;
          this->printHistory(this->closestNums.first.stepsToNum);
          std::cout << "\nOne way to get to " << std::to_string(this->closestNums.second.num) << " is: " << std::endl;
          this->printHistory(this->closestNums.second.stepsToNum);
      }
      
    }
    else
    {
      std::cout << "It is possible to get " << std::to_string(this->target) << " with the given numbers." << std::endl;
      std::cout << "One way to get to get it is: " << std::endl;
      this->printHistory(this->closestNums.first.stepsToNum);
    }
  }

  int getClosestDistance() const
  {
    return this->closestDistanceToTarget;
  }

  bool isPossible() const
  {
    return this->targetReachable;
  }
};

class LettersRoundAnswer : RoundAnswer
{

};