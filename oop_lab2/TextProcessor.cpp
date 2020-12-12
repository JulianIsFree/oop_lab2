#include <sstream>
#include <locale>

#include "TextProcessor.h"
#include "Exception.h"

labTextProcessor::WorkflowTextProcessor::WorkflowTextProcessor(const std::string & source) : 
	TextProcessor(source), source(source), fin(source)
{
	using namespace std;
	fin.imbue(locale(locale(), new DelimsLocale(delimiters)));
}

labTextProcessor::WorkflowTextProcessor::WorkflowTextProcessor(const WorkflowTextProcessor & processor)
	: WorkflowTextProcessor(processor.source) {}

labTextProcessor::WorkflowTextProcessor::~WorkflowTextProcessor()
{
	fin.close();
};

std::string labTextProcessor::WorkflowTextProcessor::getNextWord()
{
	std::string word;

	if (!fin.is_open())
		throw labException::FileNotOpened(source);
	
	if (fin.fail())
		throw labException::FileFailError(source);

	fin >> word;
	if (!isCorrect(word))
		throw labException::LexicalError(word);

	return word;
}

bool labTextProcessor::WorkflowTextProcessor::isCorrect(const std::string & word) const
{
	using namespace std;
	if (word == "=")
		return true;

	istringstream iss(word);
	iss.imbue(locale(locale(), new DelimsLocale(bannedSymbols)));

	string result;
	iss >> result;
	
	return result == word;
}

bool labTextProcessor::WorkflowTextProcessor::hasNext() const
{
	auto pos = fin.tellg();
	std::string word;
	fin >> word;
	fin.seekg(pos);

	return !word.empty();
}
