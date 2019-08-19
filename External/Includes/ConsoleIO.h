#pragma once
#include <string>
#include <vector>

namespace ConsoleIO{
	template<typename T>
	T getChoiceFromCollection(std::vector<T>, bool allowExit);
	int promptForMenuSelection(std::vector<std::string> options, bool allowExit);
	bool promptForBool(std::string phrase, std::string trueInput, std::string falseInput);
	short promptForShort(std::string phrase, short min, short max);
	int promptForInt(std::string phrase, int min, int max);
	long promptForLong(std::string phrase, long min, long max);
	long long promptForLongLong(std::string, long long min, long long max);
	float promptForFloat(std::string phrase, float min, float max);
	double promptForDouble(std::string phrase, double min, double max);
	long double promptForLongDouble(std::string phrase, long double min, long double max);
	std::string promptForInput(std::string phrase, bool allowEmpty);
	char promptForChar(std::string phrase, char min, char max);
}