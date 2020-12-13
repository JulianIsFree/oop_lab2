#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <vector>
#include "Builder.h"
#include "Worker.h"

namespace labExecutor
{
	using WorkflowWorker = labWorker::WorkflowWorker;

	class Executor
	{
	public:
		virtual void run() {};
	};

	template<class T>
	class WorkerExecutor: public Executor
	{
	protected:
		std::vector<T>& toExecute;
	public:
		WorkerExecutor(std::vector<T> &toExecute) : toExecute(toExecute) {};
	};

	class WorkflowWorkerExecutor : WorkerExecutor<WorkflowWorker*>
	{
		// Executor isn't responsible for freeing vector
		// but Builder is, if any
	public:
		WorkflowWorkerExecutor(std::vector<WorkflowWorker*>& toExecute) : WorkerExecutor(toExecute) {};
		virtual void run() override;
	};
}

#endif // !EXECUTOR_H