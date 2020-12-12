#include "Block.h"
#include "Exception.h"
#include "Constants.h"

int labBlock::getContentSize(BlockType type)
{
	switch (type)
	{
	case BlockType::READFILE:
	case BlockType::WRITEFILE:
	case BlockType::DUMP:
	case BlockType::GREP:
		return 1;
	case BlockType::REPLACE:
		return 2;
	case BlockType::EMPTY_BLOCK:
	case BlockType::SORT:
	default:
		return 0;
	}
}

labBlock::BlockType labBlock::getBlockType(const std::string & word)
{
	using namespace labConstants;
	using namespace labBlock;

	if (READFILE == word)
		return BlockType::READFILE;

	if (WRITEFILE == word)
		return BlockType::WRITEFILE;

	if (DUMP == word)
		return BlockType::DUMP;

	if (GREP == word)
		return BlockType::GREP;

	if (SORT == word)
		return BlockType::SORT;

	if (REPLACE == word)
		return BlockType::REPLACE;

	return BlockType::EMPTY_BLOCK;
}

std::string labBlock::getBlockTypeName(const BlockType & type)
{
	using namespace labConstants;
	using namespace labException;

	switch (type)
	{
	case BlockType::READFILE:
		return READFILE;
	case BlockType::WRITEFILE:
		return WRITEFILE;
	case BlockType::DUMP:
		return DUMP;
	case BlockType::GREP:
		return GREP;
	case BlockType::REPLACE:
		return REPLACE;
	case BlockType::EMPTY_BLOCK:
		return EMPTY_BLOCK;
	case BlockType::SORT:
		return SORT;
	default:
		throw ShouldNotReachThereException("Default case of getBlockTypeName in Block.h");
	}
}

size_t labBlock::getNumberOfInputs(const BlockType & type)
{
	using namespace labException;
	switch (type)
	{
	case BlockType::EMPTY_BLOCK:
	case BlockType::READFILE:
		return 0;
	case BlockType::WRITEFILE:
	case BlockType::DUMP:
	case BlockType::GREP:
	case BlockType::SORT:
	case BlockType::REPLACE:
		return 1;
	default: // case of BAD_BLOCK is also included
		throw ShouldNotReachThereException("Default case of getNumberOfInputs() in Block.h");
	}
}

size_t labBlock::getNumberOfOutputs(const BlockType & type)
{
	using namespace labException;
	switch (type)
	{
	case BlockType::EMPTY_BLOCK:
	case BlockType::WRITEFILE:
		return 0;
	case BlockType::READFILE:
	case BlockType::DUMP:
	case BlockType::GREP:
	case BlockType::SORT:
	case BlockType::REPLACE:
		return 1;
	default: // case of BAD_BLOCK is also included
		throw ShouldNotReachThereException("Default case of getNumberOfOutputs() in Block.h");
	}
}

bool labBlock::isBlockSequenceAllowed(const BlockType & first, const BlockType & second)
{
	return getNumberOfOutputs(first) == getNumberOfInputs(second);
}
