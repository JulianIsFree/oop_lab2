#include "Token.h"

std::string labToken::getTypeName(const TokenType & type)
{
	using namespace labException;
	switch (type)
	{
	case TokenType::NO_TOKEN:
		return TOKEN_NO_TOKEN;
	case TokenType::BAD_TOKEN:
		return TOKEN_BAD_TOKEN;
	case TokenType::INIT_TOKEN:
		return TOKEN_INIT_TOKEN;
	case TokenType::DESC:
		return TOKEN_DESC;
	case TokenType::CSED:
		return TOKEN_CSED;
	case TokenType::OPERATOR_NEXT:
		return TOKEN_OPERATOR_NEXT;
	case TokenType::OPERATOR_SET:
		return TOKEN_OPERATOR_SET;
	case TokenType::BLOCK_ID:
		return TOKEN_BLOCK_ID;
	case TokenType::BLOCK_TYPE:
		return TOKEN_BLOCK_TYPE;
	case TokenType::BLOCK_CONTENT:
		return TOKEN_BLOCK_CONTENT;
	}

	throw ShouldNotReachThereException("end of function std::string labToken::getTypeName()");
}
