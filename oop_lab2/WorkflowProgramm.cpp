#include <iostream>
#include "WorkflowProgramm.h"
#include "Exception.h"
void labProgramm::WorkflowProgramm::pars(int argc, char ** argv)
{
	if (argc >= 2)
		source = std::string(argv[1]);

	for (int i = 2; i < argc; ++i)
		args.push_back(std::string(argv[i]));
}
labProgramm::WorkflowProgramm::WorkflowProgramm(int argc, char * argv[]) {pars(argc, argv); };

void labProgramm::WorkflowProgramm::run()
{
	if (source.empty())
		return;

	using namespace std;
	using namespace labException;
	try 
	{
		WorkflowCompiler compiler(source);
		compiler.run();

		WorkflowBuilder builder(compiler.getBlockChain(), args);
		builder.run();
		auto builtCode = builder.getBuiltCode();
		
		WorkflowWorkerExecutor executor(builtCode);
		executor.run();
	}
	catch (Exception e)
	{
		cout << e.what() << endl;
	}
}

//int main(int argc, char **argv)
//{
//	using namespace labProgramm;
//	using namespace std;
//	
//	WorkflowProgramm programm(argc, argv);
//	programm.run();
//}