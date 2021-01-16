#include "Builder.h"
#include "Worker.h"
#include "Exception.h"

void labBuilder::WorkflowBuilder::run()
{
	using namespace labBlock;
	using namespace labException;
	using namespace labWorker;

	checkInputFlag();

	for (auto iter = blockChain.cbegin(); iter != blockChain.cend(); ++iter)
	{
		const Block block = *iter;
		auto content = block.getContent();
		switch (block.getType())
		{
		case BlockType::READFILE:
			sequence.push_back(std::shared_ptr<FileReaderWorker>(new FileReaderWorker(content)));
			break;
		case BlockType::WRITEFILE:
			sequence.push_back(std::shared_ptr<FileWriterWorker>(new FileWriterWorker(content)));
			break;
		case BlockType::DUMP:
			sequence.push_back(std::shared_ptr<FileDumpWorker>(new FileDumpWorker(content)));
			break;
		case BlockType::SORT:
			sequence.push_back(std::shared_ptr<SortWorker>(new SortWorker(content)));
			break;
		case BlockType::GREP:
			sequence.push_back(std::shared_ptr<GrepWorker>(new GrepWorker(content)));
			break;
		case BlockType::REPLACE:
			sequence.push_back(std::shared_ptr<ReplaceWorker>(new ReplaceWorker(content)));
			break;
		default:
			throw ShouldNotReachThereException("Default case of WorkflowBuilder.run()");
		}
	}

	checkOutputFlag();
}

const std::vector<std::shared_ptr<labWorker::WorkflowWorker>>& labBuilder::WorkflowBuilder::getBuiltCode() const
{
	using namespace labException;
	
	if (isCodeObtained)
		throw NoCodeToGetException();

	isCodeObtained = true;
	return sequence;
}

void labBuilder::WorkflowBuilder::checkInputFlag()
{
	using namespace labException;
	using namespace labBlock;
	using namespace labWorker;

	if (blockChain.size() > 0)
	{
		if (blockChain.begin()->getType() != BlockType::READFILE)
		{
			if (paramParser.iFlagSet)
				sequence.push_back(std::shared_ptr<FileReaderWorker>(new FileReaderWorker({ paramParser.inputFileName })));
			else
				throw NoInputFileException();
		}
	}
	else
	{
		if (paramParser.iFlagSet)
			sequence.push_back(std::shared_ptr<FileReaderWorker>(new FileReaderWorker({ paramParser.inputFileName })));
		else
			throw NoInputFileException();
	}
}

void labBuilder::WorkflowBuilder::checkOutputFlag()
{
	using namespace labBlock;
	using namespace labException;
	using namespace labWorker;
	if (blockChain.size() > 0)
	{
		if (blockChain.rbegin()->getType() != BlockType::WRITEFILE)
		{
			if (paramParser.oFlagSet)
				sequence.push_back(std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ paramParser.outputFIleName })));
			else
				throw NoInputFileException();
		}
	}
	else
	{
		if (paramParser.oFlagSet)
			sequence.push_back(std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ paramParser.outputFIleName })));
		else
			throw NoInputFileException();
	}
}

void labBuilder::WorkflowBuilder::ParamParser::pars()
{
	using namespace labException;
	// TODO: replace with iterators
	for (size_t i = 0; i < args.size(); ++i)
	{
		std::string word = args[i];
		if (word == "-i")
		{
			if (iFlagSet)
				throw BadCommandArgsException("-i flag set twice");
			if (i < args.size() - 1)
			{
				iFlagSet = true;
				inputFileName = args[++i];
			}
			else 
				throw BadCommandArgsException("input file expected");
		}
		else if (word == "-o")
		{

			if (oFlagSet)
				throw BadCommandArgsException("-o flag set twice");
			if (i < args.size() - 1)
			{
				oFlagSet = true;
				outputFIleName = args[++i];
			}
			else
				throw BadCommandArgsException("output file expected");
		}
		else
			throw BadCommandArgsException("unexpected flag");
	}
}
