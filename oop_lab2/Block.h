#ifndef BLOCK_H
#define BLOCK_H

#include<vector>
#include<string>

namespace labBlock
{
	enum class BlockType : int
	{
		BAD_BLOCK,
		EMPTY_BLOCK,
		READFILE,
		WRITEFILE,
		DUMP,
		GREP,
		SORT,
		REPLACE
	};

	int getContentSize(BlockType type);
	BlockType getBlockType(const std::string& word);

	template<class T> class _Block
	{
		BlockType type;
		std::vector<T> content;
	public:
		_Block(BlockType type=BlockType::EMPTY_BLOCK) : type(type) {};

		BlockType getType() const { return type; };
		const std::vector<T>& getContent() const { return content; };
		void addContent(T content) { this->content.push_back(content); };
	};

}
#endif // !BLOCK_H
