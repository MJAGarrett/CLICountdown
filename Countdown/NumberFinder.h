#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "OperationContext.h"
#include "RoundAnswer.h"
#include "MGHelperFunctions.h"

#ifndef NUMBERFINDER
#define NUMBERFINDER

// TODO: Account for the possibility of two potential closest answers either side of target.
// EG. If target is 150 and two possible answers are 149 and 151. May need to convert stepsToClosest into a vector storing
// the answer itself and the steps to reach it.

class NumberFinder
{
  std::vector<int> nums;
  int target;
  bool found = false;
  std::vector<std::string> tempSteps;
  int closestNum = INT_MIN;
  std::vector<std::string> stepsToClosest;
  std::vector<std::string> answerSteps;

  bool isResultClosestToTarget(const int &result) const
  {
    if (closestNum == INT_MIN) return true;

    int resultDistance = abs(this->target - result);
    int currentClosest = abs(this->target - this->closestNum);

    if (resultDistance < currentClosest) return true;
    return false;
  }

  void printHistory(const std::vector<std::string> &history)
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

  /**
   * @brief Recursively iterates through the array, testing all combinations of arithmetic operations
   * possible while avoiding redundant multiplication/addition combinations.
   * 
   * @param arr 
   */
  void find(std::vector<int> &arr)
  {
    if (arr.size() == 1 || this->found) return;

    for (int firstNumIndex = 0; firstNumIndex < arr.size(); firstNumIndex++)
    {
      if (this->found) return;
      /**
       * @brief opType 0 = addition, 1 = subtraction, 2 = multiplication, 3 = division
       * 
       */
      for (int opType = 0; opType < 4; opType++)
      {
        if (this->found) return;

        OperationContext* operation;
        switch(opType)
        {
          case 0:
            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
              operation = new AdditionContext(arr[firstNumIndex], arr[secondNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
            }
            break;

          case 1:
            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
              /********* Repetitive code throughout. Potential refactor candidate *******************************************************/
              // Potential function extraction. Could lead to stack overflow given recursive nature?
              operation = new SubtractionContext(arr[firstNumIndex], arr[secondNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
              /**************************************************************************************************************************/
            }

            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
              operation = new SubtractionContext(arr[secondNumIndex], arr[firstNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
            }
            break;
          
          case 2:
            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
                // Skips multiplying by one as that adds no value to future operations and only adds several duplicate op contexts.
                if (arr[firstNumIndex] == 1 || arr[secondNumIndex] == 1) continue;
              operation = new MultiplicationContext(arr[firstNumIndex], arr[secondNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
            }
            break;
          
          case 3:
            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
                // Skips dividing by one as that adds no value to future operations.
                if (arr[secondNumIndex] == 1) continue;
              operation = new DivisionContext(arr[firstNumIndex], arr[secondNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
            }
            for (int secondNumIndex = firstNumIndex + 1; secondNumIndex < arr.size(); secondNumIndex++)
            {
                // Skips dividing by one as that adds no value to future operations.
                if (arr[firstNumIndex] == 1) continue;
              operation = new DivisionContext(arr[secondNumIndex], arr[firstNumIndex]);
              int result = operation->getResult();
              std::string step = operation->stringify();
              delete operation;

              const int code = handleResult(result, step);

              switch (code)
              {
                case 0:
                  return;
                case 1:
                  continue;
                  break;
                case 2:
                  std::vector<int> next = MG::getNextIteration(arr, arr[firstNumIndex], arr[secondNumIndex], result);
                  if (next.size() > 1)
                    NumberFinder::find(next);
                  this->tempSteps.pop_back();
                  break;
              }
            }
            break;
        }
      }
    }
  }

  /**
   * @brief Handles a result and returns an integer code to instruct calling function on how to proceed.
   * 
   * @return int - A code instructing on how to proceed. A code of 0 means to return, 
   * a code of 1 means to continue the current loop (preventing a further recursive call), 
   * and a code of 2 means to recur with the result.
   */
  int handleResult(int result, std::string step)
  {
    if (result == this->target)
    {
      this->tempSteps.push_back(step);
      this->answerSteps = MG::copyVector(this->tempSteps);
      this->found = true;
      return 0;
    }
    else if (result <= 0) return 1;
    else 
    {
      this->tempSteps.push_back(step);

      if (this->isResultClosestToTarget(result))
      {
        this->closestNum = result;
        this->stepsToClosest = MG::copyVector(this->tempSteps);
      }
      return 2;
    }
  }

  void clearSteps()
  {
    this->tempSteps.clear();
    this->answerSteps.clear();
    this->stepsToClosest.clear();
  }
public:
  NumberFinder(int target, std::vector<int> nums) 
  {
    this->target = target;
    this->nums = nums;
  }

  MathRoundAnswer* findAnswer()
  {
    MathRoundAnswer* answer;
    this->find(this->nums);
    if (!this->found)
    {
      answer = new MathRoundAnswer(this->target, this->closestNum, this->stepsToClosest);
    }
    else 
    {
      answer = new MathRoundAnswer(this->target, this->target, this->answerSteps);
    }
    this->clearSteps();
    return answer;
  }
};

#endif