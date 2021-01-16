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
	
	vector<std::shared_ptr<WorkflowWorker>> toExecute =
	{
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({inFileName})),
		std::shared_ptr<GrepWorker>(new GrepWorker({ "test" })),
		std::shared_ptr<FileDumpWorker>(new FileDumpWorker({ subOutFileName })),
		std::shared_ptr<ReplaceWorker>(new ReplaceWorker({ "test", "develop" })),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName }))
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

TEST(WorkflowWorkerExecutor, run_write_read_mid)
{
	using namespace labExecutor;
	const string inFileName = "wwerwrm.txt";
	const vector<string> content =
	{
		"Hello",
		"It's last class to test"
	};
	const string outFileName = "wwerwrmo.txt";
	const string subOutFileName = "wwerwrmso.txt";
	createFile(inFileName, content);

	vector<std::shared_ptr<WorkflowWorker>> toExecute =
	{
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({inFileName})),
		std::shared_ptr<GrepWorker>(new GrepWorker({ "test" })),
		std::shared_ptr<FileDumpWorker>(new FileDumpWorker({ subOutFileName })),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName })),
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({ outFileName})),
		std::shared_ptr<ReplaceWorker>(new ReplaceWorker({ "test", "develop" })),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName }))
	};

	WorkflowWorkerExecutor executor(toExecute);
	EXPECT_NO_THROW(executor.run());

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

TEST(WorkflowWorkerExecutor, run_dump_read_write_read_mid)
{
	using namespace labExecutor;
	const string inFileName = "wwerwrm.txt";
	const vector<string> content =
	{
		"Hello",
		"It's last class to test"
	};
	const string outFileName = "wwerwrmo.txt";
	const string subOutFileName = "wwerwrmso.txt";
	createFile(inFileName, content);

	vector<std::shared_ptr<WorkflowWorker>> toExecute =
	{
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({inFileName})),
		std::shared_ptr<GrepWorker>(new GrepWorker({ "test" })),
		std::shared_ptr<FileDumpWorker>(new FileDumpWorker({ subOutFileName })),
		// after Dump should fail
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({ outFileName})),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName })),
		std::shared_ptr<FileReaderWorker>(new FileReaderWorker({ outFileName})),
		
		std::shared_ptr<ReplaceWorker>(new ReplaceWorker({ "test", "develop" })),
		std::shared_ptr<FileWriterWorker>(new FileWriterWorker({ outFileName }))
	};

	WorkflowWorkerExecutor executor(toExecute);
	EXPECT_ANY_THROW(executor.run());
}
