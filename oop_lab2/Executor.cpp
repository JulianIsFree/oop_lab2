#include "Executor.h"

void labExecutor::WorkflowWorkerExecutor::run()
{
	std::vector<std::string> prevOutput;
	for (auto iter = toExecute.begin(); iter != toExecute.end(); ++iter)
	{
		(*iter).operator*().setInput(prevOutput);
		(*iter).operator*().doWork();
		prevOutput = std::move(((*iter).operator*()).getOutput());
	}
}
