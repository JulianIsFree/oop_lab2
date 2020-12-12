#ifndef PARSER_H
#define PARSER_H
#include <string>
#include "Token.h"
#include "Block.h"
#include "TextProcessor.h"

namespace labParser
{
	//moved to Block.h
	//using Block = labBlock::_Block<std::string>;

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
		// may throw exceptions
		virtual void processToken(const labToken::Token& token) {};
		
		//virtual bool processWord(const std::string& word) { return processToken(getToken(word)); };
		// tells if next token must to be (csed after desc and etc)
		virtual bool isNextTokenExpected() const { return false; };
	};

	class WorkflowParser : public Parser
	{
	protected:
		// for tests
		void _setLastToken(const labToken::Token& token) { lastToken = token; };
	private:
		labBlock::Block currBlock;
		std::string currBlockName;
		labToken::Token lastToken;
		bool csedReached;

		void processBlock(const labToken::Token& token);
		void resetBlock();
	public:
		
		WorkflowParser() : lastToken(labToken::TokenType::INIT_TOKEN, ""), csedReached(false) {};
		virtual labToken::Token getToken(const std::string& word) const override; // no tests required
		virtual bool isTokenAllowed(const labToken::Token& token) const override; // no tests required
		
		// Once block is ready receiving next token by this function will destroy previous block.  
		virtual void processToken(const labToken::Token& token) override; // tested
		
		virtual bool isNextTokenExpected() const override; // tested
		bool isEmptyBlock(); // no tests required

		bool isBlockComplete() const; // tested

		const labBlock::Block& getBlock() const;
		std::string getBlockName() const { return currBlockName; };
	};
}
#endif // !PARSER_H
