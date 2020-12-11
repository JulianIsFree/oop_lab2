#ifndef COMPILER_H
#define COMPILER_H
#include <map>
#include <vector>
#include <string>

#include "Parser.h"
#include "LexicalAnalyzer.h"
#include "Block.h"

using namespace labParser;
using namespace labLexicalAnalyzer;

namespace labCompiler
{
	template <class T> 
	class Compiler
	{
	public:
		Compiler(const std::string&) {};
		virtual ~Compiler() {};

		virtual void run() {};
		virtual std::vector<T> getBlockChain() { return std::vector<T>(); };
		virtual std::map<std::string, T> getBlocks() { return std::map<std::string, T>(); };
	};

	class WorkflowCompiler : public Compiler<labParser::Block>
	{
	private:
		std::vector<labParser::Block> blockChain; // chain to execute
		std::map<std::string, labParser::Block> blockTable;	// table: (identifier, block)
		
		// TODO: rename LexicalAnalyzer to TextProcessor
		LexicalAnalyzer analyzer;
		Parser parser;
	public:
		WorkflowCompiler(const std::string& sourceFile);
	};
}

#endif // !COMPILER_H
