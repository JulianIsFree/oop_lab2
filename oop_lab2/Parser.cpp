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
	
	if (isBlockComplete() && !isEmptyBlock())
		resetBlock();

	if (TokenType::BLOCK_CONTENT == token.getType())
		currBlock.addContent(token.getWord());
	else if (TokenType::BLOCK_TYPE == token.getType())
		currBlock = Block(getBlockType(token.getWord()));
	else if (TokenType::BLOCK_ID == token.getType())
		currBlockName = token.getWord();
}

void labParser::WorkflowParser::resetBlock()
{
	currBlock = labBlock::Block();
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

	if (TokenType::OPERATOR_SET == lastToken.getType())
		return Token(TokenType::BLOCK_TYPE, word);

	if (TokenType::OPERATOR_NEXT == lastToken.getType() ||
		TokenType::DESC == lastToken.getType() || 
		TokenType::CSED == lastToken.getType() )
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

void labParser::WorkflowParser::processToken(const labToken::Token & token)
{
	using namespace labToken;
	using namespace labException;

	if (!isTokenAllowed(token))
		throw BadTokenException(token);
	
	if (token == TokenType::CSED)
		csedReached = true;

	processBlock(token);
	lastToken = token;
}

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
		return false;
	case TokenType::BLOCK_ID:
		return !csedReached;
	case TokenType::BLOCK_TYPE:
	case TokenType::BLOCK_CONTENT:
		return !isBlockComplete();
	default:
		return true;
	}
}

bool labParser::WorkflowParser::isEmptyBlock()
{
	return currBlock.getType() == labBlock::BlockType::EMPTY_BLOCK;
}

const labBlock::Block & labParser::WorkflowParser::getBlock() const
{
	using namespace labException;
	if (!isBlockComplete())
		throw UnfinishedBlockException("uncompleted block");

	return currBlock;
}
