#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <fstream>
#include <vector>

namespace labLexicalAnalyzer
{
	struct DelimsLocale : std::ctype<char>
	{
		DelimsLocale(std::string const &delims) : std::ctype<char>(get_table(delims)) {}

		static std::ctype_base::mask const* get_table(std::string const &delims)
		{
			static std::vector<std::ctype_base::mask> delimiters(table_size, std::ctype_base::mask());
			for (unsigned char ch : delims)
				delimiters[ch] = std::ctype_base::space;

			return &delimiters[0];
		}
	};

	class LexicalAnalyzer
	{
	public:
		LexicalAnalyzer(const std::string&) {};
		virtual std::string getNextWord() { return std::string(); };
		virtual bool isCorrect(const std::string&) const { return false; };
		virtual bool hasNext() const { return false; };
	};

	static const std::string delimiters = " \n";
	static const std::string bannedSymbols = delimiters + "_-+=)({}[]";
	
	class WorkflowLexicalAnalyzer : LexicalAnalyzer
	{
		std::ifstream fin;
		std::string source;
	public:
		WorkflowLexicalAnalyzer(const std::string& source);
		virtual ~WorkflowLexicalAnalyzer();

		virtual std::string getNextWord();
		virtual bool isCorrect(const std::string& word) const override;
		virtual bool hasNext() const override;
	};
}

#endif // !LEXICAL_ANALYZER_H
