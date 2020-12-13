#include "Executor.h"

void labExecutor::WorkflowWorkerExecutor::run()
{
	std::vector<std::string> prevOutput;
	for (auto iter = toExecute.begin(); iter != toExecute.end(); ++iter)
	{
		(*iter)->setInput(prevOutput);
		(*iter)->doWork();
		prevOutput = std::move((*iter)->getOutput());
	}
}
