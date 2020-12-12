#include "Compiler.h"
#include "Exception.h"
void labCompiler::WorkflowCompiler::doBackUp()
{
	blockChain = std::move(chainBackUp);
	blockTable = std::move(tableBackUp);
}

void labCompiler::WorkflowCompiler::setBackUp()
{
	chainBackUp = blockChain;
	tableBackUp = blockTable;
}

void labCompiler::WorkflowCompiler::processBlockSegment()
{
	using namespace labException;
	using namespace labBlock;
	using namespace labToken;
	using namespace std;

	bool isCSEDReached = false;
	while (!isCSEDReached)
	{
		if (!processor.hasNext())
			throw UnexpectedEndOfFileException("CSED expected");

		string word = processor.getNextWord();
		Token token = parser.getToken(word);
		parser.processToken(token);

		if (token.getType() == TokenType::CSED)
			isCSEDReached = true;

		if (parser.isBlockComplete())
		{
			if (blockTable.find(parser.getBlockName()) != blockTable.end())
				throw BlockRedefenitionException(parser.getBlockName());
			
			if(!parser.isEmptyBlock())
				blockTable[parser.getBlockName()] = Block(parser.getBlock());
		}
	}
}

// only 2 kinds of TokenTypes are allowed to be: BLOCK_ID and OPERATOR_NEXT
void labCompiler::WorkflowCompiler::processCodeSegment()
{
	using namespace labToken;
	using namespace labBlock;
	using namespace labException;
	using namespace std;
	
	// First iteration is out of while-cycle
	string word = processor.getNextWord();
	Token token = parser.getToken(word);
	parser.processToken(token);
	
	// Even if parser throws an exception while calling processToken(), 
	// compiler doesn't know about this in abstract meaning
	if (token.getType() != TokenType::BLOCK_ID)
		throw BadTokenException(token);

	const string name = word; // this "redefinition" needs only for better understanding code
	if (blockTable[name].getType() == BlockType::EMPTY_BLOCK)
		throw BlockDoesNotExistsException(name);
	blockChain.push_back(blockTable[name]);

	while (processor.hasNext())
	{
		word = processor.getNextWord();
		token = parser.getToken(word);
		parser.processToken(token);

		if (token.getType() == TokenType::BLOCK_ID)
		{
			const string name = word;
			BlockType newBlockType = blockTable[name].getType();

			if (newBlockType == BlockType::EMPTY_BLOCK)
				throw BlockDoesNotExistsException(name);
			
			BlockType lastBlockType = blockChain[blockChain.size() - 1].getType();

			if (!isBlockSequenceAllowed(lastBlockType, newBlockType))
				throw BlockSequenceIsNotAllowedException(
					getBlockTypeName(lastBlockType), getBlockTypeName(newBlockType));

			if (blockTable.find(name) == blockTable.end())
				throw BlockNotFoundException(name);

			blockChain.push_back(blockTable[name]);
		}
	}

	if (parser.isNextTokenExpected())
		throw UnexpectedEndOfFileException("BLOCK_ID expected");
}

labCompiler::WorkflowCompiler::WorkflowCompiler(const std::string & sourceFile) :
	Compiler(sourceFile),
	processor(WorkflowTextProcessor(sourceFile)),
	parser(WorkflowParser()) {}

void labCompiler::WorkflowCompiler::run()
{
	using namespace labException;
	setBackUp();
	
	try
	{
		// parsing block-segment
		processBlockSegment();
		
		// parsing code-segment
		processCodeSegment();
	}
	catch (Exception e)
	{
		doBackUp();
		throw e;
	}
}

