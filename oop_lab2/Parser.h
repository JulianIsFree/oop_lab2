#pragma once

#include <fstream>
#include <string>
#include <map>

/*
readfile <filename>  Ц считывание текстового файла в пам€ть, целиком.
¬ход Ц отсутствует, выход Ц текст.
writefile <filename> Ц запись текста в файл.
¬ход Ц текст, выход Ц отсутствует.
grep <word> Ц выбор из входного текста строк, разделенных символами переноса строки, содержащих заданное слово <word>.
¬ход Ц текст, выход Ц текст.
sort Ц лексикографическа€ сортировка входного набора строк.
¬ход Ц текст, выход Ц текст.
replace <word1> <word2> Ц замена слова word1 словом word2 во входном тексте.
¬ход Ц текст, выход Ц текст.
dump <filename> - сохранить пришедший текст в указанном файле и передать дальше.
*/

namespace labParser
{
	// with every blockName added need to update str2block and block2str functions
	enum class BlockName : unsigned char 
	{
		EMPTY_BLOCK,
		READFILE, 
		WRITEFILE, 
		GREP, 
		SORT, 
		REPLACE, 
		DUMP
	};
	
	BlockName str2block(const std::string& word);
	std::string block2str(const BlockName& blockName);


	// with every added Token need do update functions convertStr2Token and convertToken2str
	// of Analyser class. Also need to update KeyWords.h if any constant strings required.
	enum class Token : unsigned char 
	{
		BAD_TOKEN,
		NO_TOKEN,		// initial token
		BLOCK_ID,		// identifier associated with block
		BLOCK_TYPE,		// enum class BlockName
		BLOCK_CONTENT,	// arguments for block defined in task
		OPERATOR_SET,	// =
		OPERATOR_NEXT,	// ->
		DESC,			// describe section start
		CSED			// describe section end
	};

	class Block
	{
	private:
		BlockName blockName;
		std::vector<std::string> content;
	public:
		Block(BlockName& blockName, const std::vector<std::string>& content) : blockName(blockName),
			content(std::vector<std::string>(content)) {};
		Block() : blockName(BlockName::EMPTY_BLOCK), content(std::vector<std::string>()) {};
		Block(const Block& block);

		static bool hasContent(const BlockName& name);

		const std::vector<std::string>& getContent() const;
		BlockName getBlockName() const;

		void setBlockName(const BlockName& blockName);
		void setContent(const std::vector<std::string>& content);
		void addContent(const std::string& content);

		Block& operator=(const Block& block);

		operator std::string();
	};

	class Analyser
	{
	private:
		Token lastTokenInBlockSection;
		Token lastTokenInSequenceSection;
		bool sequenceSectionLogicEndReached;

	public:
		Analyser() : lastTokenInBlockSection(Token::NO_TOKEN), 
					 lastTokenInSequenceSection(Token::NO_TOKEN),
					 sequenceSectionLogicEndReached(true) {};
		// check if using "token" after "lastToken" is allowed, semantic check
		bool doSemanticCheckForBlockSection(const Token& token); 
		bool doSemanticCheckForSequenceSection(const Token& token);

		bool isBlockIDLastInSequenceSection() const;

		const Token& getLastTokenInBlockSection() const;
		const Token& getLastTokenInSequenceSection() const;

		Token convertStr2Token(const std::string& str, const Block& block=Block()) const;
		std::string convertToken2Str(const Token& token) const;
	};

	class Parser
	{
	private:
		Analyser analyser;
		std::string fileName;
		std::map<std::string, Block> blockTable;
		std::vector<Block> sequence;

		void collectBlocks(std::ifstream& fin);
		void collectSequence(std::ifstream& fin);
		void resetState();
	public:
		Parser(const std::string& fileName);
		Parser() {};
		void run();
		void setFileName(const std::string& fileName);
		const std::vector<Block>& getBlockSequence() const; // To interact with interpreter
		const std::map<std::string, Block>& getBlockTable() const;
	};
}