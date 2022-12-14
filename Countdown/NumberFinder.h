#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include "OperationContext.h"
#include "RoundAnswer.h"
#include "MGHelperFunctions.h"
#include "ClosestNumContext.h"

#ifndef NUMBERFINDER
#define NUMBERFINDER

class NumberFinder
{
  std::vector<int> nums;
  int target;
  bool found = false;
  std::vector<std::string> tempSteps;
  std::pair<ClosestNumContext, ClosestNumContext> closestNums = { ClosestNumContext(INT_MIN), ClosestNumContext(INT_MAX) };

  bool isResultClosestToTarget(const int &result) const
  {
      if (result > this->target)
      {
          if (result < closestNums.second.num)
              return true;
      }
      else 
      {
          if (result > closestNums.first.num)
              return true;
      }
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
      this->closestNums.first = ClosestNumContext(result, this->tempSteps);
      this->closestNums.second = ClosestNumContext(result, this->tempSteps);
      this->found = true;
      return 0;
    }
    else if (result <= 0) return 1;
    else 
    {
      this->tempSteps.push_back(step);

      if (this->isResultClosestToTarget(result))
      {
          if (result > this->target) 
          {
              this->closestNums.second.num = result;
              this->closestNums.second.stepsToNum = MG::copyVector(this->tempSteps);
          }
          else
          {
              this->closestNums.first.num = result;
              this->closestNums.first.stepsToNum = MG::copyVector(this->tempSteps);
          }
      }
      return 2;
    }
  }

  void clearSteps()
  {
    this->tempSteps.clear();
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
    answer = new MathRoundAnswer(this->target, this->closestNums);
    this->clearSteps();
    return answer;
  }
};

#endif