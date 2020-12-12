#ifndef BUILDER_H
#define BUILDER_H
#include <vector>
#include "Worker.h"
#include "Block.h"

namespace labBuilder
{
	class Builder
	{
	public:
		virtual void run() {};
		virtual std::vector<labWorker::WorkflowWorker*> getBuiltCode() { return std::vector<labWorker::WorkflowWorker*>(); };
	};

	// 1 builder per code-sequence
	class WorkflowBuilder : public Builder
	{
		const std::vector<labBlock::Block>& blockChain;
		std::vector<labWorker::WorkflowWorker*> sequence;
	public:
		WorkflowBuilder(const std::vector<labBlock::Block>& blockChain) : blockChain(blockChain) {};
		virtual void run() override;
		virtual std::vector<labWorker::WorkflowWorker*> getBuiltCode() override { return sequence; };
	};
}

#endif // !BUILDER_H
