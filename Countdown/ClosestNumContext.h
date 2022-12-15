#include <string>
#include <vector>

#pragma once

struct ClosestNumContext
{
	int num;
	std::vector<std::string> stepsToNum = {};
	ClosestNumContext(int num) : num(num) {};
	ClosestNumContext(int num, std::vector<std::string> steps) : num(num), stepsToNum(steps) {};
};