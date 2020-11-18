#include "pch.h"
#include "Parser.h"
#include "Helper.h"
#include "Exceptions.h"
#include "KeyWords.h"

using namespace labExceptions;
using namespace labParser;

Parser::Parser(const std::string & fileName)
{
	this->fileName = fileName;
	analyser = Analyser();
}

void Parser::run()
{
	std::ifstream fin(fileName);
	resetState();

	if (!fin.is_open())
		throw FileInputError(codeHelper::fileNotOpened(fileName));
	
	if(fin.fail())
		throw FileInputError("error occured with file " + fileName);

	fin.imbue(std::locale(std::locale(), new codeHelper::DelimsLocale(" \n"))); 
	try
	{
		collectBlocks(fin);
		collectSequence(fin);
	}
	catch (Exception e)
	{
		resetState();
		e.printMessage();
		throw e;
	}

	fin.close();
}

void labParser::Parser::setFileName(const std::string & fileName)
{
	this->fileName = fileName;
}

const std::vector<Block>& Parser::getBlockSequence() const
{
	return sequence;
}

const std::map<std::string, Block>& labParser::Parser::getBlockTable() const
{
	return blockTable;
}

void Parser::collectBlocks(std::ifstream& fin)
{
	if (!blockTable.empty() || !sequence.empty() ||
		analyser.getLastTokenInBlockSection() != Token::NO_TOKEN ||
		analyser.getLastTokenInSequenceSection() != Token::NO_TOKEN)
		throw ParserStateError("parser state isn't reset");

	if (!fin.is_open())
		throw FileInputError(codeHelper::fileNotOpened(fileName));
	
	std::vector<std::string> content;
	std::string blockID;
	BlockName blockName = BlockName::EMPTY_BLOCK;
	Block block = Block();
	
	bool endReached = false;
	while (!endReached)
	{
		if (fin.eof())
			throw CSEDNotFound("unexpected end of file");
		
		if (fin.fail())
			throw FileInputError("error occured with file while parsing: " + fileName);

		std::string word;
		fin >> word;
		Token token = analyser.convertStr2Token(word, block);

		if (!analyser.doSemanticCheckForBlockSection(token))
			throw BadToken("token " + word + " isn't allowed after " + analyser.convertToken2Str(analyser.getLastTokenInBlockSection()));
		
		switch (token)
		{
		case Token::DESC:
			continue;
		case Token::BLOCK_ID:
			blockID = word;
			break;
		case Token::BLOCK_TYPE:
			blockName = str2block(word);
			block.setBlockName(blockName);
			if (blockName == BlockName::EMPTY_BLOCK)
				throw BadBlock("block-type: " + word + "isn't expected");

			if (false == Block::hasContent(str2block(word)))
			{
				blockTable[blockID] = block;
				blockID.clear();
				content.clear();
				blockName = BlockName::EMPTY_BLOCK;
				block = Block();
			}
			break;
		case Token::BLOCK_CONTENT:
			if (false == Block::hasContent(block.getBlockName()))
				throw BadToken("block " + block2str(block.getBlockName()) + " mustn't have any content");

			if (blockTable.find(blockID) != blockTable.end())
				throw BlockRedefiniton(blockID);

			content.push_back(word);
			block.addContent(word);
			if (block.getBlockName() != BlockName::REPLACE 
				|| content.size() == 2) // if blockName == replace && content.size() == 1 then no block assign required
			{
				blockTable[blockID] = block;
				blockID.clear();
				content.clear();
				blockName = BlockName::EMPTY_BLOCK;
				block = Block();
			}
			break;
		case Token::OPERATOR_SET:
			continue;
		case Token::CSED:
			endReached = true;
			break;
		default:
			throw Exception("fatal error: " + blockID + "::" +  std::string(block));
		}
	}
}

void labParser::Parser::collectSequence(std::ifstream & fin)
{
	std::string word;
	while (fin >> word)
	{
		Token token = analyser.convertStr2Token(word);

		if (!analyser.doSemanticCheckForSequenceSection(token))
			throw BadToken("token: " + analyser.convertToken2Str(token) + " isn't allowed after token: " + analyser.convertToken2Str(analyser.getLastTokenInSequenceSection()));
		
		switch (token)
		{
		case Token::BLOCK_ID:
			if (blockTable.find(word) == blockTable.end())
				throw BlockNotFound(word);

			sequence.push_back(blockTable[word]);
			break;
		case Token::OPERATOR_NEXT:
			continue;
		default:
			throw BadToken("unexpected token " + analyser.convertToken2Str(token) + " in sequence section");
		}
	}

	if (!analyser.isBlockIDLastInSequenceSection())
		throw BadToken("unexpected end of sequence section, last token: " + analyser.convertToken2Str(analyser.getLastTokenInSequenceSection()));
}

void Parser::resetState()
{
	blockTable.clear();
	sequence.clear();
	analyser = Analyser();
}

bool labParser::Analyser::doSemanticCheckForBlockSection(const Token & token)
{
	Token prevLastToken = lastTokenInBlockSection;
	lastTokenInBlockSection = token;

	switch (prevLastToken)
	{
	case Token::NO_TOKEN:
		return token == Token::DESC;
	case Token::DESC:
		return token == Token::BLOCK_ID;
	case Token::BLOCK_ID:
		return token == Token::OPERATOR_SET;
	case Token::OPERATOR_SET:
		return token == Token::BLOCK_TYPE;
	case Token::BLOCK_TYPE:
		return token == Token::BLOCK_ID ||
			token == Token::BLOCK_CONTENT ||
			token == Token::CSED;
	case Token::BLOCK_CONTENT:
		return token == Token::BLOCK_CONTENT ||
			token == Token::BLOCK_ID ||
			token == Token::CSED;
	default:
		return false;
	}
}

bool labParser::Analyser::doSemanticCheckForSequenceSection(const Token & token)
{
	Token prevLastToken = lastTokenInSequenceSection;
	lastTokenInSequenceSection = token;
	switch (prevLastToken)
	{
	case Token::BLOCK_ID:
		return token == Token::OPERATOR_NEXT;
	case Token::NO_TOKEN:
	case Token::OPERATOR_NEXT:
		return token == Token::BLOCK_ID;
	default:
		return false;
	}
}

bool labParser::Analyser::isBlockIDLastInSequenceSection() const
{
	return  lastTokenInSequenceSection == Token::NO_TOKEN ||
			lastTokenInSequenceSection == Token::BLOCK_ID;
}

const Token & labParser::Analyser::getLastTokenInBlockSection() const
{
	return lastTokenInBlockSection;
}

const Token & labParser::Analyser::getLastTokenInSequenceSection() const
{
	return lastTokenInSequenceSection;
}

Token labParser::Analyser::convertStr2Token(const std::string & str, const Block& block) const
{
	if (str == labKeyWords::OPERATOR_NEXT)
		return Token::OPERATOR_NEXT;

	if (str == labKeyWords::OPERATOR_SET)
		return Token::OPERATOR_SET;

	if (str == labKeyWords::DESC)
		return Token::DESC;

	if (str == labKeyWords::CSED)
		return Token::CSED;

	if (labKeyWords::isBlockType(str))
		return Token::BLOCK_TYPE;

	// if sequence section hasn't been reached, then there may be a content for a block
	if (lastTokenInSequenceSection == Token::NO_TOKEN)
	{
		if ((block.getBlockName() == BlockName::REPLACE && block.getContent().size() < 2) ||
			(Block::hasContent(block.getBlockName()) && block.getContent().size() < 1))
			return Token::BLOCK_CONTENT;
	}
	
	return Token::BLOCK_ID;
}

std::string labParser::Analyser::convertToken2Str(const Token & token) const
{
	if (token == Token::OPERATOR_NEXT)
		return labKeyWords::OPERATOR_NEXT;

	if (token == Token::OPERATOR_SET)
		return labKeyWords::OPERATOR_SET;

	if (token == Token::DESC)
		return labKeyWords::DESC;
	
	if (token == Token::CSED)
		return labKeyWords::CSED;

	if (token == Token::BLOCK_ID)
		return "BLOCK_ID"; // there is no specific return for BLOCK_ID token
	
	if (token == Token::BLOCK_TYPE)
		return "BLOCK_TYPE"; // as for the BLOCK_ID token

	if (token == Token::BLOCK_CONTENT)
		return "BLOCK_CONTENT"; // as for BLOCK_ID token

	if (token == Token::NO_TOKEN)
		return "NO_TOKEN"; // as for BLOCK_ID

	return "BAD_TOKEN";
}

BlockName labParser::str2block(const std::string & word)
{
	if (word == labKeyWords::READFILE)
		return BlockName::READFILE;
	
	if (word == labKeyWords::WRITEFILE)
		return BlockName::WRITEFILE;
	
	if (word == labKeyWords::GREP)
		return BlockName::GREP;
	
	if (word == labKeyWords::SORT)
		return BlockName::SORT;
	
	if (word == labKeyWords::REPLACE)
		return BlockName::REPLACE;

	if (word == labKeyWords::DUMP)
		return BlockName::DUMP;

	return BlockName::EMPTY_BLOCK;
}

std::string labParser::block2str(const BlockName & blockName)
{
	if (blockName == BlockName::DUMP)
		return labKeyWords::DUMP;

	if (blockName == BlockName::REPLACE)
		return labKeyWords::REPLACE;

	if (blockName == BlockName::SORT)
		return labKeyWords::SORT;

	if (blockName == BlockName::GREP)
		return labKeyWords::GREP;

	if (blockName == BlockName::WRITEFILE)
		return labKeyWords::WRITEFILE;

	if (blockName == BlockName::READFILE)
		return labKeyWords::READFILE;

	return labKeyWords::EMPTY_BLOCK;
}

labParser::Block::Block(const Block & block)
{
	this->content = block.getContent();
	this->blockName = block.getBlockName();
}

const std::vector<std::string>& labParser::Block::getContent() const
{
	return content;
}

BlockName labParser::Block::getBlockName() const
{
	return blockName;
}

void labParser::Block::setBlockName(const BlockName & blockName)
{
	this->blockName = blockName;
}

void labParser::Block::setContent(const std::vector<std::string>& content)
{
	this->content = std::vector<std::string>(content);
}

void labParser::Block::addContent(const std::string & content)
{
	this->content.push_back(content);
}

Block & labParser::Block::operator=(const Block & block)
{
	this->content = std::vector<std::string>(block.getContent());
	this->blockName = block.getBlockName();
	return *this;
}

bool labParser::Block::hasContent(const BlockName & name)
{
	return 
		name == BlockName::WRITEFILE ||
		name == BlockName::READFILE ||
		name == BlockName::REPLACE ||
		name == BlockName::GREP ||
		name == BlockName::DUMP;
}

labParser::Block::operator std::string()
{
	std::string contentStr = "[";
	for (auto iter = content.cbegin(); iter != content.cend(); ++iter)
		contentStr += *iter + ",";

	if (contentStr[contentStr.length() - 1] == ',')
		contentStr[contentStr.length() - 1] = ']';
	else
		contentStr.push_back(']');

	return block2str(blockName) + "::" + contentStr;
}
