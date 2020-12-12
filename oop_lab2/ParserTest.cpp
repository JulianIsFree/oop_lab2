#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "Parser.h"
#include "TextProcessor.h"
#include "Helper.h"

using namespace std;
using namespace labHelper;
using namespace labToken;

TEST(WorkflowParser, processToken) // it's also a test for getToken, isTokenAllowed and e.t.c.
{
	using namespace labTextProcessor;
	using namespace labParser;

	const string fileName = "WorkflowParserGetToken.txt";
	const vector<string> content = {"desc", "name1 = readfile file.txt", "name2 = grep", "csed"};
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	WorkflowParser parser;
	string word;
	Token token;
	
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::DESC);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word); 
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_CONTENT);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::CSED);
	parser.processToken(token);
}

TEST(WorkflowParser, isNextTokenExpected)
{
	using namespace labTextProcessor;
	using namespace labParser;
	const string fileName = "WorkflowParserIsNextTokenExpected.txt";
	const vector<string> content = { "desc", "name1 = sort", "name2 = readfile file", "csed", "name1 -> name2" };
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	WorkflowParser parser;

	string word;
	Token token;

	EXPECT_EQ(parser.isNextTokenExpected(), false);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::DESC);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), false);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_CONTENT);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), false);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::CSED);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), false);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), false);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_NEXT);
	parser.processToken(token);

	EXPECT_EQ(parser.isNextTokenExpected(), true);
	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);
}

TEST(WorkflowParser, isBlockComplete)
{
	using namespace labTextProcessor;
	using namespace labParser;

	const string fileName = "WorkflowParserIsBlockComplete.txt";
	const vector<string> content = { "desc", "name1 = sort", "name2 = readfile file", "csed" };
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	WorkflowParser parser;
	string word;
	Token token;

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::DESC);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_ID);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::OPERATOR_SET);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_TYPE);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), false);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::BLOCK_CONTENT);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);

	word = processor.getNextWord();
	token = parser.getToken(word);
	EXPECT_EQ(token, TokenType::CSED);
	parser.processToken(token);
	EXPECT_EQ(parser.isBlockComplete(), true);
}