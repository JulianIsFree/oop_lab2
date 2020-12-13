#ifndef WORKFLOWPROGRAMM_H
#define WORKFLOWPROGRAMM_H
#include <string>

#include "Builder.h"
#include "Executor.h"
#include "Compiler.h"

using namespace labBuilder;
using namespace labExecutor;
using namespace labCompiler;

namespace labProgramm
{
	class WorkflowProgramm
	{
		std::string source;
		std::vector<std::string> args;
		void pars(int argc, char**argv);
	public:
		WorkflowProgramm(int argc, char *argv[]);
		void run();
	};
}


#endif