#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <vector>
#include <memory>
#include "Builder.h"
#include "Worker.h"

namespace labExecutor
{
	using WorkflowWorkerSmartPointer = labWorker::WorkflowWorkerSmartPointer;

	class Executor
	{
	public:
		virtual void run() {};
	};

	template<class T>
	class WorkerExecutor: public Executor
	{
	protected:
		std::vector<T> toExecute;
	public:
		WorkerExecutor(const std::vector<T> &toExecute) : toExecute(toExecute) {};
	};

	class WorkflowWorkerExecutor : WorkerExecutor<std::shared_ptr<labWorker::WorkflowWorker>>
	{
		// Executor isn't responsible for freeing vector
		// but Builder is, if any
	public:
		WorkflowWorkerExecutor(const std::vector<std::shared_ptr<labWorker::WorkflowWorker>>& toExecute) : WorkerExecutor(toExecute) {};
		virtual void run() override;
	};
}

#endif // !EXECUTOR_H