#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "Executor.h"

#include "Builder.h"
#include "Worker.h"
#include "Helper.h"

using namespace labBuilder;
using namespace labWorker;
using namespace labHelper;
using namespace std;

TEST(WorkflowWorkerExecutor, run)
{
	using namespace labExecutor;
	const string inFileName = "WorkflowWorkerExecutorRun.txt";
	const vector<string> content = 
	{
		"Hello",
		"It's last class to test"
	};
	const string outFileName = "WorkflowWorkerExecutorRunResult.txt";
	const string subOutFileName = "WorkflowWorkerExecutorRunSubResult.txt";
	createFile(inFileName, content);
	FileReaderWorker id0({inFileName});
	GrepWorker id1({ "test" });
	FileDumpWorker id2({ subOutFileName });
	ReplaceWorker id3({ "test", "develop" });
	FileWriterWorker id4({ outFileName });
	
	vector<WorkflowWorker*> toExecute =
	{
		&id0,
		&id1,
		&id2,
		&id3,
		&id4
	};

	WorkflowWorkerExecutor executor(toExecute);
	executor.run();

	FileReaderWorker helper({ outFileName });
	helper.doWork();

	auto result = helper.getOutput();
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], "It's last class to develop");

	FileReaderWorker subHelper({ subOutFileName });
	subHelper.doWork();

	auto subResult = subHelper.getOutput();
	EXPECT_EQ(subResult.size(), 1);
	EXPECT_EQ(subResult[0], content[1]);
}
