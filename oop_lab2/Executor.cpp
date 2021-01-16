#include "Executor.h"
#include "Exception.h"

void labExecutor::WorkflowWorkerExecutor::run()
{
	std::vector<std::string> prevOutput;

	auto iter = toExecute.begin();
	(*iter)->setInput(prevOutput);
	(*iter)->doWork();
	

	prevOutput = std::move((*iter)->getOutput());
	auto prevIter = iter++;
	/*
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({inFileName})),
		std::shared_ptr<GrepWorker>(new GrepWorker({ "test" })),
		std::shared_ptr<FileDumpWorker>(new FileDumpWorker({ subOutFileName })),
		std::shared_ptr<ReplaceWorker>(new ReplaceWorker({ "test", "develop" })),
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({inFileName})),
		std::shared_ptr<ReplaceWorker>(new ReplaceWorker({ "test", "develop" })),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName }))
	*/
	for (; iter != toExecute.end(); ++iter)
	{
		auto t1 = (*prevIter)->getBlockType();
		auto t2 = (*iter)->getBlockType();
		if (!labBlock::isBlockSequenceAllowed(t1, t2))
			throw labException::BlockSequenceIsNotAllowedException(labBlock::getBlockTypeName(t1), labBlock::getBlockTypeName(t2));

		(*iter)->setInput(prevOutput);
		(*iter)->doWork();
		prevOutput = std::move(((*iter).operator*()).getOutput());
		prevIter = iter;
	}
}
