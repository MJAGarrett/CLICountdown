#include <vector>
#include <iostream>
#include "OperationContext.h"
#include <sstream>
#include "MGHelperFunctions.h"
#include <unordered_map>

#pragma once

class MathRoundInputReader 
{
  int target = 0;
  int playerBest = INT_MIN;
  std::vector<int> validNums;
  std::unordered_map<std::string, OpType> symbolToOp = {
    {"*", multiplication},
    {"/", division},
    {"+", addition},
    {"-", subtraction},
  };
  bool keepQuerying = true;

  OperationContext* getOpBySymbol(std::string symbol, int arg1, int arg2)
  {
    try
    {
      auto lookup = this->symbolToOp.find(symbol);
      OpType opType = lookup->second;
      OperationContext* op = nullptr;
      switch(opType)
      {
        case addition:
          op = new AdditionContext(arg1, arg2);
          break;
        case subtraction:
          op = new SubtractionContext(arg1, arg2);
          if (op->getResult() == INT_MIN) throw std::runtime_error("Subtraction not possible");
          break;
        case multiplication:
          op =  new MultiplicationContext(arg1, arg2);
          break;
        case division:
          op = new DivisionContext(arg1, arg2);
          if (op->getResult() == INT_MIN) throw std::runtime_error("Division not possible");
          break;
      }
      if (op == nullptr) throw std::runtime_error("Operation type not initialized");
      return op;
    }  
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      throw e;
    }
  }

  void checkPlayerAnswer(std::string answer)
  {
    std::string token;
    std::istringstream ss(answer);
    std::vector<std::string> tokens;

    while(std::getline(ss, token, ' '))
    {
      tokens.push_back(token);
    }
    
    if (tokens.size() != 3)
    {
      throw std::runtime_error("Bad input");
    }
    OperationContext* op = nullptr;
    try
    {
      int x = std::stoi(tokens[0]);
      int y = std::stoi(tokens[2]);
      if (!(MG::vectorContainsBoth(this->validNums, x, y))) throw std::runtime_error("Operand(s) not valid");
      op = getOpBySymbol(tokens[1], x, y);
      int result = op->getResult();

      this->playerBest = MG::getClosest(this->target, this->playerBest, result);
      this->validNums = MG::getNextIteration(this->validNums, x, y, result);
      delete op;
    }
    catch(const std::exception& e)
    {
      if (op != nullptr) delete op;
      std::cerr << e.what() << '\n';
    }
    
  }

  void queryAnswer()
  {
    std::string answer;
    std::cout << "Enter an operation in the form of 'Number Operation Number'; for example '10 * 15' or '2 + 3'." << std::endl;
    std::cout << "Enter 'done' to exit" << std::endl;
    std::getline(std::cin, answer);

    if (answer == "done")
    {
      this->keepQuerying = false;
      return;
    }
    try
    {
      this->checkPlayerAnswer(answer);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }   
  }

  void beginQuery()
  {
    while (this->playerBest != this->target && this->validNums.size() > 1 && this->keepQuerying)
    {
      std::cout << "\nTarget number: " << std::to_string(this->target) << std::endl;
      std::cout << "Current numbers you may use: ";
      for (int val : this->validNums)
      {
        std::cout << std::to_string(val) << ", ";
      }
      std::cout << "\n" << std::endl;
      queryAnswer();
    }
  }

  void reset()
  {
    this->playerBest = INT_MIN;
    this->validNums = {};
    this->keepQuerying = true;
    this->target = 0;
  }

public:

  //TODO: Setup questioning. Getting big/small numbers.

    int askForNumbers()
    {
        std::string answer;
        int numOfBigs = INT_MIN;

        std::cout << "Select how many big numbers you would like. You may pick between 0 and 4." << std::endl;
        std::cout << "Please enter your choice as a single numeric digit. E.G. '1', '2', etc." << std::endl;

        while (numOfBigs == INT_MIN)
        {
            std::getline(std::cin, answer);
            try
            {
                int selection = std::stoi(answer);
                if (selection > 4 || selection < 0) std::cout << "Please enter a number between 0 and 4" << std::endl;
                else numOfBigs = selection;
            }
            catch (std::exception& err)
            {
                std::cout << "Could not parse your input. Please enter a number between 0 and 4." << std::endl;
            }
        }
        return numOfBigs;
    }

  int startUp(int target, std::vector<int> choices)
  {
    this->target = target;
    this->validNums = choices;

    this->beginQuery();
    int bestAnswer = this->playerBest;
    this->reset();

    return bestAnswer;
  }
};