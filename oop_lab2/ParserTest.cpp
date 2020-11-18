#include "pch.h"
#include <fstream>
#include <vector>
#include <string>
#include "Exceptions.h"
#include "KeyWords.h"
#include "Parser.h"

TEST(BlockName, str2block)
{
	using namespace labParser;

	EXPECT_EQ(str2block("readfile"), BlockName::READFILE);
	EXPECT_EQ(str2block("writefile"), BlockName::WRITEFILE);
	EXPECT_EQ(str2block("grep"), BlockName::GREP);
	EXPECT_EQ(str2block("sort"), BlockName::SORT);
	EXPECT_EQ(str2block("replace"), BlockName::REPLACE);
	EXPECT_EQ(str2block("dump"), BlockName::DUMP);
	
	EXPECT_NE(str2block("Readfile"), BlockName::READFILE);
	EXPECT_NE(str2block("Writefile"), BlockName::WRITEFILE);
	EXPECT_NE(str2block("gRep"), BlockName::GREP);
	EXPECT_NE(str2block("soRt"), BlockName::SORT);
	EXPECT_NE(str2block("rePlace"), BlockName::REPLACE);
	EXPECT_NE(str2block("dumP"), BlockName::DUMP);

	EXPECT_EQ(str2block("Readfile"), BlockName::EMPTY_BLOCK);
	EXPECT_EQ(str2block("Writefile"), BlockName::EMPTY_BLOCK);
	EXPECT_EQ(str2block("gRep"), BlockName::EMPTY_BLOCK);
	EXPECT_EQ(str2block("soRt"), BlockName::EMPTY_BLOCK);
	EXPECT_EQ(str2block("rePlace"), BlockName::EMPTY_BLOCK);
	EXPECT_EQ(str2block("dumP"), BlockName::EMPTY_BLOCK);
}

TEST(BlockName, block2str)
{
	using namespace labParser;

	EXPECT_EQ(block2str(BlockName::READFILE), labKeyWords::READFILE);
	EXPECT_EQ(block2str(BlockName::WRITEFILE), labKeyWords::WRITEFILE);
	EXPECT_EQ(block2str(BlockName::GREP), labKeyWords::GREP);
	EXPECT_EQ(block2str(BlockName::SORT), labKeyWords::SORT);
	EXPECT_EQ(block2str(BlockName::REPLACE), labKeyWords::REPLACE);
	EXPECT_EQ(block2str(BlockName::DUMP), labKeyWords::DUMP);
	EXPECT_EQ(block2str(BlockName::EMPTY_BLOCK), labKeyWords::EMPTY_BLOCK);
}

TEST(Analyser, doSemanticCheckForBlockSection)
{
	using namespace labParser;
	Analyser analyser = Analyser();

	std::vector<Token> tokenStream;
	tokenStream.push_back(Token::DESC);

	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_SET);
	tokenStream.push_back(Token::BLOCK_TYPE);

	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_SET);
	tokenStream.push_back(Token::BLOCK_TYPE);
	tokenStream.push_back(Token::BLOCK_CONTENT);

	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_SET);
	tokenStream.push_back(Token::BLOCK_TYPE);
	tokenStream.push_back(Token::BLOCK_CONTENT);
	tokenStream.push_back(Token::BLOCK_CONTENT);

	tokenStream.push_back(Token::CSED);

	for (auto iter = tokenStream.cbegin(); iter != tokenStream.cend(); ++iter)
		EXPECT_EQ(true, analyser.doSemanticCheckForBlockSection(*iter));
}

TEST(Analyser, doSemanticCheckForSequenceSection)
{
	using namespace labParser;
	Analyser analyser;
	std::vector<Token> tokenStream;

	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_NEXT);
	
	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_NEXT);

	tokenStream.push_back(Token::BLOCK_ID);
	tokenStream.push_back(Token::OPERATOR_NEXT);

	for (auto iter = tokenStream.cbegin(); iter != tokenStream.cend(); ++iter)
		EXPECT_EQ(true, analyser.doSemanticCheckForSequenceSection(*iter));
}

TEST(Analyser, fullSemanticCheck)
{
	using namespace labParser;
	Analyser analyser;
	std::vector<Token> blockSection;
	blockSection.push_back(Token::DESC);

	blockSection.push_back(Token::BLOCK_ID);
	blockSection.push_back(Token::OPERATOR_SET);
	blockSection.push_back(Token::BLOCK_TYPE);

	blockSection.push_back(Token::BLOCK_ID);
	blockSection.push_back(Token::OPERATOR_SET);
	blockSection.push_back(Token::BLOCK_TYPE);
	blockSection.push_back(Token::BLOCK_CONTENT);

	blockSection.push_back(Token::BLOCK_ID);
	blockSection.push_back(Token::OPERATOR_SET);
	blockSection.push_back(Token::BLOCK_TYPE);
	blockSection.push_back(Token::BLOCK_CONTENT);
	blockSection.push_back(Token::BLOCK_CONTENT);

	blockSection.push_back(Token::CSED);

	for (auto iter = blockSection.cbegin(); iter != blockSection.cend(); ++iter)
		EXPECT_EQ(true, analyser.doSemanticCheckForBlockSection(*iter));

	std::vector<Token> sequenceStream;

	sequenceStream.push_back(Token::BLOCK_ID);
	sequenceStream.push_back(Token::OPERATOR_NEXT);

	sequenceStream.push_back(Token::BLOCK_ID);
	sequenceStream.push_back(Token::OPERATOR_NEXT);

	sequenceStream.push_back(Token::BLOCK_ID);
	sequenceStream.push_back(Token::OPERATOR_NEXT);

	for (auto iter = sequenceStream.cbegin(); iter != sequenceStream.cend(); ++iter)
		EXPECT_EQ(true, analyser.doSemanticCheckForSequenceSection(*iter));
}

TEST(Analyser, convertStr2Token)
{
	using namespace labParser;
	Analyser analyser;

	std::vector<std::string> text;
	text.push_back("desc"); // 0
	
	text.push_back("code1");
	text.push_back("=");
	text.push_back("readfile");
	text.push_back("input.txt");
	
	text.push_back("code2");
	text.push_back("=");
	text.push_back("sort");

	text.push_back("code3");
	text.push_back("=");
	text.push_back("replace");
	text.push_back("word1");
	text.push_back("word2");

	text.push_back("csed"); // 13

	text.push_back("code1"); //14
	text.push_back("->");
	text.push_back("code2");
	text.push_back("->");
	text.push_back("code3"); //18

	std::vector<Token> tokens;
	Token token;
	Block block;
	
	token = analyser.convertStr2Token(text[0], block);
	EXPECT_EQ(token, Token::DESC);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	
	token = analyser.convertStr2Token(text[1], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[2], block);
	EXPECT_EQ(token, Token::OPERATOR_SET);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[3], block);
	EXPECT_EQ(token, Token::BLOCK_TYPE);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.setBlockName(str2block(text[3]));

	token = analyser.convertStr2Token(text[4], block);
	EXPECT_EQ(token, Token::BLOCK_CONTENT);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.addContent(text[4]);
	block = Block();

	token = analyser.convertStr2Token(text[5], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[6], block);
	EXPECT_EQ(token, Token::OPERATOR_SET);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[7], block);
	EXPECT_EQ(token, Token::BLOCK_TYPE);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.setBlockName(str2block(text[7]));
	block = Block();

	token = analyser.convertStr2Token(text[8], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[9], block);
	EXPECT_EQ(token, Token::OPERATOR_SET);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	
	token = analyser.convertStr2Token(text[10], block);
	EXPECT_EQ(token, Token::BLOCK_TYPE);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.setBlockName(str2block(text[10]));

	token = analyser.convertStr2Token(text[11], block);
	EXPECT_EQ(token, Token::BLOCK_CONTENT);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.addContent(text[11]);

	token = analyser.convertStr2Token(text[12], block);
	EXPECT_EQ(token, Token::BLOCK_CONTENT);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);
	block.addContent(text[11]);
	block = Block();

	token = analyser.convertStr2Token(text[13], block);
	EXPECT_EQ(token, Token::CSED);
	EXPECT_EQ(analyser.doSemanticCheckForBlockSection(token), true);

	token = analyser.convertStr2Token(text[14], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForSequenceSection(token), true);

	token = analyser.convertStr2Token(text[15], block);
	EXPECT_EQ(token, Token::OPERATOR_NEXT);
	EXPECT_EQ(analyser.doSemanticCheckForSequenceSection(token), true);

	token = analyser.convertStr2Token(text[16], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForSequenceSection(token), true);

	token = analyser.convertStr2Token(text[17], block);
	EXPECT_EQ(token, Token::OPERATOR_NEXT);
	EXPECT_EQ(analyser.doSemanticCheckForSequenceSection(token), true);

	token = analyser.convertStr2Token(text[18], block);
	EXPECT_EQ(token, Token::BLOCK_ID);
	EXPECT_EQ(analyser.doSemanticCheckForSequenceSection(token), true);
}

TEST(Analyser, isBlockIDLastInSequenceSection)
{
	using namespace labParser;
	Analyser analyser;
	EXPECT_EQ(true, analyser.isBlockIDLastInSequenceSection());

	analyser.doSemanticCheckForSequenceSection(Token::BLOCK_ID);
	EXPECT_EQ(true, analyser.isBlockIDLastInSequenceSection());

	analyser.doSemanticCheckForSequenceSection(Token::OPERATOR_NEXT);
	EXPECT_EQ(false, analyser.isBlockIDLastInSequenceSection());

	analyser.doSemanticCheckForSequenceSection(Token::BLOCK_ID);
	EXPECT_EQ(true, analyser.isBlockIDLastInSequenceSection());
}

void createTestFile(const std::string& fileName)
{
	using namespace std;
	ofstream fout(fileName);
	if (!fout.is_open())
		throw "Can't create test file with name " + fileName;

	fout << "desc" << endl;
	fout << "code1 = readfile input.txt" << endl;
	fout << "code2 = sort" << endl;
	fout << "code3 = replace word1 word2" << endl;
	fout << "csed" << endl;
	fout << "code1 -> code2 -> code3" << endl;

	fout.close();
}

TEST(Parser, run)
{
	createTestFile("test.txt");
	
	using namespace labParser;
	using namespace labExceptions;

	Parser parser("test.txt");
	EXPECT_NO_THROW(parser.run());

	std::map<std::string, Block> blockTable = parser.getBlockTable();
	EXPECT_EQ(std::string(blockTable["code1"]), "readfile::[input.txt]");
	EXPECT_EQ(std::string(blockTable["code2"]), "sort::[]");
	EXPECT_EQ(std::string(blockTable["code3"]), "replace::[word1,word2]");
}