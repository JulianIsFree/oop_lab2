#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <fstream>
#include <vector>
#include "Helper.h"
using namespace labHelper;

namespace labTextProcessor
{
	class TextProcessor
	{
	public:
		TextProcessor(const std::string&) {};
		virtual std::string getNextWord() { return std::string(); };
		virtual bool isCorrect(const std::string&) const { return false; };
		virtual bool hasNext() const { return false; };
	};

	static const std::string delimiters = " \n";
	static const std::string bannedSymbols = "_+)({}[]"; 
	
	class WorkflowTextProcessor : public TextProcessor
	{
	private:
		mutable std::ifstream fin;
		std::string source;
	public:
		WorkflowTextProcessor(const std::string& source);
		WorkflowTextProcessor(const WorkflowTextProcessor& processor);
		virtual ~WorkflowTextProcessor();

		virtual std::string getNextWord() override;
		// TODO: It may be a bug in my own design
		// Make function isCorrect doesn't depend on imbue, it may cause undefined behavior
		// or save locales since isCorrect is called and restore at the end of this.
		// imbue affects STATIC locales!!! to be changed. 
		// That means even if you call imbue on stream 'a',
		// and after this call imbue on stream 'b', for 'b' there will be same locales as for 'a' plus 
		// b's own locales.
		virtual bool isCorrect(const std::string& word) const override;
		virtual bool hasNext()const override;
	};
}

#endif // !LEXICAL_ANALYZER_H
