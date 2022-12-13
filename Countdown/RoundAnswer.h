#include <string>
#include <vector>
#include <iostream>

#pragma once

class RoundAnswer
{
public:
  virtual void printAnswer() const = 0;
};

class MathRoundAnswer : RoundAnswer
{
  const int target;
  const int numReached;
  const std::vector<std::string> steps;
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
public:
  MathRoundAnswer(int target, int numReached, std::vector<std::string> steps) : target(target), numReached(numReached), steps(steps) {}
  void printAnswer() const
  {
    if (!(this->target == this->numReached))
    {
      std::cout << "It is not possible to get " << std::to_string(this->target) << " with the given numbers." << std::endl;
      std::cout << "The closest one can get to " << std::to_string(this->target) << " is " << std::to_string(this->numReached) << "." << std::endl;
      std::cout << "One way to get to " << std::to_string(this->numReached) + " is:" << std::endl;
      this->printHistory(this->steps);
    }
    else
    {
      std::cout << "It is possible to get " << std::to_string(this->target) << " with the given numbers." << std::endl;
      std::cout << "One way to get to get it is: " << std::endl;
      this->printHistory(this->steps);
    }
  }

  int getAnswer()
  {
    return numReached;
  }

  bool isPossible()
  {
    return this->target == this->numReached;
  }
};

class LettersRoundAnswer : RoundAnswer
{

};