#include "Parser.h"
#include "Constants.h"
#include "Exception.h"

bool labParser::WorkflowParser::isBlockComplete() const
{
	return currBlock.getContent().size() == labBlock::getContentSize(currBlock.getType());
}

void labParser::WorkflowParser::processBlock(const labToken::Token& token)
{
	using namespace labToken;
	using namespace labBlock;

	if (TokenType::BLOCK_TYPE == token.getType())
		currBlock = Block(getBlockType(token.getWord()));

	if (TokenType::BLOCK_CONTENT == token.getType())
		currBlock.addContent(token.getWord());
}

void labParser::WorkflowParser::reset()
{
	currBlock = Block();
	currBlockName = "";
}

// this is not the fastest way to calculate token type
// but the easiest one in case of implementation 
labToken::Token labParser::WorkflowParser::getToken(const std::string & word) const
{
	using namespace labToken;
	using namespace labConstants;

	if (OPERATOR_NEXT == word)
		return Token(TokenType::OPERATOR_NEXT, word);

	if (OPERATOR_SET == word)
		return Token(TokenType::OPERATOR_SET, word);

	if (DESC == word)
		return Token(TokenType::DESC, word);

	if (CSED == word)
		return Token(TokenType::CSED, word);

	if (TokenType::OPERATOR_SET == lastToken.getType() ||
		TokenType::DESC == lastToken.getType() || 
		TokenType::CSED == lastToken.getType() )
		return Token(TokenType::BLOCK_TYPE, word);

	if (TokenType::OPERATOR_NEXT == lastToken.getType())
		return Token(TokenType::BLOCK_ID, word);

	if (!isBlockComplete())
		return Token(TokenType::BLOCK_CONTENT, word);

	return Token(TokenType::BLOCK_ID, word);
}

bool labParser::WorkflowParser::isTokenAllowed(const labToken::Token & token) const
{
	using namespace labToken;
	TokenType type = token.getType();
	switch (lastToken.getType())
	{
	case TokenType::INIT_TOKEN:
		return type == TokenType::DESC;
	case TokenType::DESC:
		return type == TokenType::CSED || type == TokenType::BLOCK_ID;
	case TokenType::CSED:
	case TokenType::OPERATOR_NEXT:
		return type == TokenType::BLOCK_ID;
	case TokenType::OPERATOR_SET:
		return type == TokenType::BLOCK_TYPE;
	case TokenType::BLOCK_ID:
		return type == TokenType::OPERATOR_NEXT || type == TokenType::OPERATOR_SET;
	case TokenType::BLOCK_TYPE:
	case TokenType::BLOCK_CONTENT:
		return type == TokenType::BLOCK_CONTENT || type == TokenType::BLOCK_ID ||
			type == TokenType::CSED;
	default:
		return false;
	}
}

bool labParser::WorkflowParser::processToken(const labToken::Token & token)
{
	using namespace labToken;

	bool result = isTokenAllowed(token);
	processBlock(token);
	lastToken = token;
	
	return result;
}

//bool labParser::WorkflowParser::processWord(const std::string & word)
//{
//	return processToken(getToken(word));
//}

bool labParser::WorkflowParser::isNextTokenExpected() const
{
	using namespace labToken;
	TokenType type = lastToken.getType();
	switch (type)
	{
	case TokenType::BAD_TOKEN:
	case TokenType::NO_TOKEN:
	case TokenType::INIT_TOKEN:
	case TokenType::CSED:
	case TokenType::BLOCK_ID:
		return false;
	case TokenType::BLOCK_TYPE:
	case TokenType::BLOCK_CONTENT:
		return isBlockComplete();
	default:
		return true;
	}
}

const labParser::Block & labParser::WorkflowParser::getBlock() const
{
	using namespace labException;
	if (!isBlockComplete())
		throw BadBlockException("uncompleted block");

	return currBlock;
}
