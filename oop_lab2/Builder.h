#ifndef BUILDER_H
#define BUILDER_H
#include <vector>
#include "Worker.h"
#include "Block.h"

namespace labBuilder
{
	template<class T>
	class Builder
	{
	public:
		virtual void run() {};
	};

	// 1 builder per code-sequence
	// and once it's ready-to-execute code is obtained it must throw an exception on other return
	class WorkflowBuilder : public Builder<labWorker::WorkflowWorkerSmartPointer>
	{
		class ParamParser
		{
			friend class WorkflowBuilder;
			std::vector<std::string> args;

			std::string inputFileName;
			std::string outputFIleName;
			bool iFlagSet;
			bool oFlagSet;

			void pars();
		public:
			ParamParser(const std::vector<std::string>& args) : args(args), iFlagSet(false), oFlagSet(false) { pars(); };
		};
		ParamParser paramParser;
		const std::vector<labBlock::Block>& blockChain;
		std::vector<labWorker::WorkflowWorkerSmartPointer> sequence;
		mutable bool isCodeObtained;

		void checkInputFlag();
		void checkOutputFlag();
	public:
		WorkflowBuilder(const std::vector<labBlock::Block>& blockChain) : 
			blockChain(blockChain), paramParser(std::vector <std::string>()), isCodeObtained(false) {};
		WorkflowBuilder(const std::vector<labBlock::Block>& blockChain, const std::vector<std::string>& args) : 
			blockChain(blockChain), paramParser(args), isCodeObtained(false) {};

		virtual void run() override;
		const std::vector<labWorker::WorkflowWorkerSmartPointer>& getBuiltCode() const;
	};
}

#endif // !BUILDER_H
