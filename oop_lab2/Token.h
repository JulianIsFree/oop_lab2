#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "Exception.h"
namespace labToken
{
	enum class TokenType : unsigned int
	{
		NO_TOKEN,
		BAD_TOKEN,
		INIT_TOKEN,
		DESC,
		CSED,
		OPERATOR_NEXT,
		OPERATOR_SET,
		BLOCK_ID,
		BLOCK_TYPE,
		BLOCK_CONTENT 
	};

	static const std::string TOKEN_NO_TOKEN = "NO_TOKEN";
	static const std::string TOKEN_BAD_TOKEN = "BAD_TOKEN";
	static const std::string TOKEN_INIT_TOKEN = "INIT_TOKEN";
	static const std::string TOKEN_DESC = "DESC_TOKEN";
	static const std::string TOKEN_CSED = "TOKEN_CSED";
	static const std::string TOKEN_OPERATOR_NEXT = "TOKEN_OPERATOR_NEXT";
	static const std::string TOKEN_OPERATOR_SET = "TOKEN_OPERATOR_SET";
	static const std::string TOKEN_BLOCK_ID = "TOKEN_BLOCK_ID";
	static const std::string TOKEN_BLOCK_TYPE = "TOKEN_BLOCK_TYPE";
	static const std::string TOKEN_BLOCK_CONTENT = "TOKEN_BLOCK_CONTENT";

	std::string getTypeName(const TokenType& type);
	
	class Token
	{
		TokenType type;
		std::string word;
	public:
		Token(TokenType type = TokenType::NO_TOKEN, const std::string& word = "") : type(type), word(word) {};
		Token& operator=(const Token& token) { type = token.type; word = token.word; return *this; };
		TokenType getType() const { return type; };
		const std::string& getWord() const { return word; };
		operator std::string() const { return getTypeName(type); };
		operator TokenType() const { return type; };
	};
}

#endif // !TOKEN_H
