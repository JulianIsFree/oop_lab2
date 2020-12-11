#include <sstream>
#include <locale>

#include "LexicalAnalyzer.h"
#include "Exception.h"

labLexicalAnalyzer::WorkflowLexicalAnalyzer::WorkflowLexicalAnalyzer(const std::string & source) : LexicalAnalyzer(source), source(source), fin(source)
{
	using namespace std;
	fin.imbue(locale(locale(), new DelimsLocale(delimiters)));
}

labLexicalAnalyzer::WorkflowLexicalAnalyzer::~WorkflowLexicalAnalyzer()
{
	fin.close();
};

std::string labLexicalAnalyzer::WorkflowLexicalAnalyzer::getNextWord()
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

bool labLexicalAnalyzer::WorkflowLexicalAnalyzer::isCorrect(const std::string & word) const
{
	using namespace std;

	istringstream iss(word);
	iss.imbue(locale(locale(), new DelimsLocale(bannedSymbols)));

	string result;
	iss >> result;

	return result == word;
}

bool labLexicalAnalyzer::WorkflowLexicalAnalyzer::hasNext() const
{
	return !fin.eof();
}
