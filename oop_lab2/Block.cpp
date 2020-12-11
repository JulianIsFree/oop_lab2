#include "Block.h"
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
