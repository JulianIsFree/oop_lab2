#ifndef TOKEN_H
#define TOKEN_H
#include <string>

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

	class Token
	{
		TokenType type;
		std::string word;
	public:
		Token(TokenType type = TokenType::NO_TOKEN, const std::string& word = "") : type(type), word(word) {};
		Token& operator=(const Token& token) { type = token.type; word = token.word; return *this; };
		TokenType getType() const { return type; };
		const std::string& getWord() const { return word; };
	};
}

#endif // !TOKEN_H
