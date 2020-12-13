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
		virtual std::vector<T*> getBuiltCode() const { return std::vector<labWorker::WorkflowWorker*>(); };
	};

	// 1 builder per code-sequence
	// and once it's ready-to-execute code is obtained it must throw an exception on other return
	class WorkflowBuilder : public Builder<labWorker::WorkflowWorker>
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
		std::vector<labWorker::WorkflowWorker*> sequence;
		mutable bool isCodeObtained;

		void checkInputFlag();
		void checkOutputFlag();
	public:
		WorkflowBuilder(const std::vector<labBlock::Block>& blockChain) : 
			blockChain(blockChain), paramParser(std::vector <std::string>()), isCodeObtained(false) {};
		WorkflowBuilder(const std::vector<labBlock::Block>& blockChain, const std::vector<std::string>& args) : 
			blockChain(blockChain), paramParser(args), isCodeObtained(false) {};

		~WorkflowBuilder();
		virtual void run() override;
		virtual std::vector<labWorker::WorkflowWorker*> getBuiltCode() const override;
	};
}

#endif // !BUILDER_H
