#ifndef COMPILER_H
#define COMPILER_H
#include <map>
#include <list>
#include <string>

#include "Parser.h"
#include "TextProcessor.h"
#include "Block.h"

using namespace labParser;
using namespace labTextProcessor;

namespace labCompiler
{
	class Compiler
	{
	public:
		Compiler(const std::string&) {};
		virtual ~Compiler() {};
		virtual void run() {};
	};

	// compiler isn't responsible for exception processing
	// and it doesn't compile anything at all
	// the name of the class just represents the idea
	class WorkflowCompiler : public Compiler
	{
	private:
		// TODO: change to std::list
		std::list<labBlock::Block> blockChain; // chain to execute
		std::map<std::string, labBlock::Block> blockTable;	// table: (identifier, block)

		/* Both parser and analyzer must guarantee strong exception safety 
			...but they don't.
		Since it'a problem I would find out solution in the future.
		TODO: make Workflow-things guarantee strong exception safety */
		
		WorkflowTextProcessor processor;
		WorkflowParser parser;

		// for strong exception safety
		std::list<labBlock::Block> chainBackUp;
		std::map<std::string, labBlock::Block> tableBackUp;
		void doBackUp();
		void setBackUp();

		class CompilerTester;
		friend class CompilerTester;

		void processBlockSegment();
		void processCodeSegment();
	public:
		WorkflowCompiler(const std::string& sourceFile);
		virtual void run() override;
		const std::list<labBlock::Block>& getBlockChain() const { return blockChain; };
		const std::map<std::string, labBlock::Block>& getBlocks() const { return blockTable; };
	};
}

#endif // !COMPILER_H
