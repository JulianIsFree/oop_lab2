#include "Builder.h"
#include "Worker.h"
#include "Exception.h"

void labBuilder::WorkflowBuilder::run()
{
	using namespace labBlock;
	using namespace labException;
	using namespace labWorker;

	for (auto iter = blockChain.cbegin(); iter != blockChain.cend(); ++iter)
	{
		const Block block = *iter;
		auto content = block.getContent();
		switch (block.getType())
		{
		case BlockType::READFILE:
			sequence.push_back(new FileReaderWorker(content));
			break;
		case BlockType::WRITEFILE:
			sequence.push_back(new FileWriterWorker(content));
			break;
		case BlockType::DUMP:
			sequence.push_back(new FileDumpWorker(content));
			break;
		case BlockType::SORT:
			sequence.push_back(new SortWorker(content));
			break;
		case BlockType::GREP:
			sequence.push_back(new GrepWorker(content));
			break;
		case BlockType::REPLACE:
			sequence.push_back(new ReplaceWorker(content));
			break;
		default:
			throw ShouldNotReachThereException("Default case of WorkflowBuilder.run()");
		}
	}
}
