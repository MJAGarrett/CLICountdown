// Countdown.cpp : A simple CLI game of countdown.
//
#include "NumberFinder.h"
#include "MathRound.h"
#include "Game.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>

/* Functions and variables for manual testing begin here */
std::vector<int> bigNums = { 100, 75, 50, 25 };
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

std::random_device rd;
std::mt19937 gen(rd());

void addSmallNum(std::vector<int>& nums)
{
	auto it = smallNums.begin();
	std::uniform_int_distribution<> randIndex(0, (smallNums.size() - 1));
	int index = randIndex(gen);
	it += index;
	nums.push_back(smallNums[index]);
	smallNums.erase(it);
}

void addBigNum(std::vector<int>& nums)
{
	if (bigNums.size() == 0)
	{
		addSmallNum(nums);
	}
	else
	{
		auto it = bigNums.begin();
		std::uniform_int_distribution<> randIndex(0, (bigNums.size() - 1));
		int index = randIndex(gen);
		it += index;
		nums.push_back(bigNums[index]);
		bigNums.erase(it);
	}
}

std::vector<int> getRandomNums(std::uniform_real_distribution<>& zeroToOne)
{
	std::vector<int> nums;

	for (int i = 0; i < 6; i++)
	{
		double coinFlip = zeroToOne(gen);
		if (coinFlip < .5)
		{
			addSmallNum(nums);
		}
		else
		{
			addBigNum(nums);
		}
	}
	return nums;
}

void refreshNums()
{
	bigNums = { 100, 75, 50, 25 };
	smallNums = {
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

void testFinder(int rounds)
{
	std::uniform_int_distribution<> dist(100, 999);
	std::uniform_real_distribution<> zeroToOne(0.0, 1.0);

	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < rounds; i++)
	{
		int target = dist(gen);
		std::cout << "Target is " << std::to_string(target) << ".\nNumbers are : [ ";
		std::vector<int> nums = getRandomNums(zeroToOne);
		for (auto num : nums)
		{
			std::cout << std::to_string(num) << ", ";
		}
		std::cout << "]." << std::endl;
		NumberFinder finder(target, nums);
		finder.findAnswer();

		std::cout << std::endl;

		refreshNums();
	}
	auto end = std::chrono::steady_clock::now();

	std::cout << "operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "sec" << std::endl;

};

void startGame(MathRound& round)
{
	round.addNum(big);
	round.addNum(big);
	round.addNum(big);
	round.addNum(small);
	round.addNum(small);
	round.addNum(small);

	round.startRound();
};

/* Manual testing section ends here */

int main(int argc, char const* argv[])
{
	Game game = Game();
	game.playGame();

	return 0;
}