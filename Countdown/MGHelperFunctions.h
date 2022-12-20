#include <vector>
#include <unordered_map>
#include <set>
#pragma once

namespace MG
{
	/**
	 * @brief Gets the next int vector for processing
	 *
	 * @param current
	 * @param firstNum
	 * @param otherNum
	 * @param result
	 * @return std::vector<int>
	 */
	std::vector<int> getNextIteration(const std::vector<int>& current, const int firstNum, const int otherNum, const int result)
	{
		std::vector<int> next;
		next.push_back(result);

		if (firstNum == otherNum)
		{
			int removedNum = 0;
			auto begin = current.begin();
			auto end = current.end();

			while (begin != end)
			{
				if (*begin == firstNum && removedNum < 2)
				{
					begin++;
					removedNum++;
					continue;
				}
				else {
					next.push_back(*begin);
					begin++;
				}
			}

			return next;
		}

		bool alreadyRemovedFirst = false;
		bool alreadyRemovedOther = false;

		for (int num : current)
		{
			if ((num != firstNum && num != otherNum) || (num == otherNum && alreadyRemovedOther) || (num == firstNum && alreadyRemovedFirst))
			{
				next.push_back(num);
			}
			else
			{
				if (num == firstNum)
				{
					alreadyRemovedFirst = true;
				}
				else if (num == otherNum)
				{
					alreadyRemovedOther = true;
				}
			}
		}

		return next;
	}

	template<class T>
	std::vector<T> removeElementFromVector(const std::vector<T>& vector, T valToRemove)
	{
		bool removedVal = false;
		std::vector<T> copyVector;

		for (T valToCopy : vector)
		{
			if (valToCopy == valToRemove && !removedVal)
			{
				removedVal = true;
				continue;
			}
			copyVector.push_back(valToCopy);
		}
		return copyVector;
	}

	template<class t>
	std::vector<t> copyVector(const std::vector<t>& initial)
	{
		std::vector<t> copy;
		for (const t val : initial)
		{
			copy.push_back(val);
		}

		return copy;
	}

	int getClosest(const int& target, const int& first, const int& second)
	{
		if (first == second) return first;
		if (first == INT_MIN) return second;
		if (second == INT_MIN) return first;

		int firstDistance = abs(target - first);
		int secondDistance = abs(target - second);

		if (firstDistance < secondDistance) return first;
		return second;
	}

	template<class T>
	bool vectorContainsBoth(const std::vector<T>& vec, const T& arg1, const T& arg2)
	{
		if (arg1 == arg2)
		{
			int howMany = 0;
			auto begin = vec.begin();
			while (howMany != 2 && begin != vec.end())
			{
				if (*begin == arg1)
				{
					howMany++;
				}
				begin++;
			}
			if (howMany == 2) return true;
			return false;
		}

		auto begin = vec.begin();
		bool hasArg1 = false;
		bool hasArg2 = false;

		while (!(hasArg1 && hasArg2) && begin != vec.end())
		{
			if (*begin == arg1) hasArg1 = true;
			else if (*begin == arg2) hasArg2 = true;
			begin++;
		}
		if (hasArg1 && hasArg2) return true;
		return false;
	}
};