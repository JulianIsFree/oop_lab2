#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Token.h"
#include "Block.h"
#include "LexicalAnalyzer.h"

namespace labParser
{
	using Block = labBlock::_Block<std::string>;

	// Syntax and semantic check
	class Parser
	{
	public:
		Parser() {};

		// convert word to token
		virtual labToken::Token getToken(const std::string&) const { return labToken::Token(); };
		
		// perfoms syntax and semantic check
		virtual bool isTokenAllowed(const labToken::Token& token) const { return false; };
		
		// updates state of analyzer (if any) and returns isTokenAllowed(token)
		// may throws exceptions
		virtual bool processToken(const labToken::Token& token) { return isTokenAllowed(token); };
		
		//virtual bool processWord(const std::string& word) { return processToken(getToken(word)); };
		// tells if next token must to be (csed after desc and etc)
		virtual bool isNextTokenExpected() const { return false; };
	};

	class WorkflowParser : Parser
	{
		Block currBlock;
		std::string currBlockName;
		labToken::Token lastToken;

		bool isBlockComplete() const;
		void processBlock(const labToken::Token& token);
		void reset();
	public:
		WorkflowParser() : lastToken(labToken::TokenType::INIT_TOKEN, "") {};
		virtual labToken::Token getToken(const std::string& word) const override;
		virtual bool isTokenAllowed(const labToken::Token& token) const override;
		virtual bool processToken(const labToken::Token& token) override;
		
		// combining getToken and processToken
		//virtual bool processWord(const std::string& word);
		virtual bool isNextTokenExpected() const;
		const Block& getBlock() const;
	};
}
#endif // !PARSER_H
