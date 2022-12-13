#include <string>
#include <functional>
#include <stdexcept>

#ifndef OPERATIONCONTEXT
#define OPERATIONCONTEXT


int add(int x, int y) 
{
  // Return to prevent overflow errors.
  if (INT_MAX - x < y) return INT_MIN;
  return x + y;
}

int sub(int x, int y) 
{
  // Returns INT_MIN to prevent negative numbers entering operation path and (unlikely) overflow errors.
  if (INT_MIN + x > y || x - y < 0) return INT_MIN;
  return x - y;
}

int mult(int x, int y) 
{
  /**
   * Returns INT_MIN to avoid processing operation paths that add no value to search.
   * Multiplying a number by 1 does not add any value to future operations and merely
   * adds duplicate search contexts to total amount of searches.
   */
  if (INT_MAX / y < x) return INT_MIN;
  return x * y;
}

int divide(int x, int y)
{
  /**
   * Returns INT_MIN to avoid processing unnecessary operation paths and division by innapropriate
   * divisor. Dividing by one adds no value to future searches.
   */
  if (y == 0 || x % y > 0)
  {
    return INT_MIN;
  }
  return x / y;
}

enum OpType
{
  addition,
  multiplication,
  division,
  subtraction,
};

class OperationContext 
{
protected:
  const std::string verb;
  const int arg1;
  const int arg2;
  const int result;
public:
  OperationContext(std::function<int(int, int)> op, std::string verb, int arg1, int arg2) 
    : verb(verb), arg1(arg1), arg2(arg2), result(op(arg1, arg2)) {}

  int getResult() const
  {
    return this->result;
  }

  virtual std::string stringify() const
  {
    std::string arg1 = std::to_string(this->arg1);
    std::string arg2 = std::to_string(this->arg2);
    std::string result = std::to_string(this->result);

    std::string interpolated = this->verb + " " + arg1 + " by " + arg2 + " to get a result of " + result;
    return interpolated;
  };

};

class AdditionContext : public OperationContext
{
public:
  AdditionContext(int arg1, int arg2) : OperationContext(&add, "add", arg1, arg2) {}

  std::string stringify() const
  {
    std::string arg1 = std::to_string(this->arg1);
    std::string arg2 = std::to_string(this->arg2);
    std::string result = std::to_string(this->result);

    std::string interpolated = this->verb + " " + arg2 + " to " + arg1 + " to get a result of " + result;
    return interpolated;
  }
};

class SubtractionContext : public OperationContext
{
public:
  SubtractionContext(int arg1, int arg2) : OperationContext(&sub, "subtract", arg1, arg2) {}

  std::string stringify() const
  {
    std::string arg1 = std::to_string(this->arg1);
    std::string arg2 = std::to_string(this->arg2);
    std::string result = std::to_string(this->result);

    std::string interpolated = this->verb + " " + arg2 + " from " + arg1 + " to get a result of " + result;
    return interpolated;
   }
};

class MultiplicationContext : public OperationContext
{
public:
  MultiplicationContext(int arg1, int arg2) : OperationContext(&mult, "multiply", arg1, arg2) {}

};

class DivisionContext : public OperationContext
{
public:
  DivisionContext(int arg1, int arg2) : OperationContext(&divide, "divide", arg1, arg2) {}
  
};

#endif