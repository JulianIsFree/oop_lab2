#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "Worker.h"
#include "Helper.h"

using namespace std;
using namespace labHelper;
TEST(FileReaderWorker, doWork)
{
	using namespace labWorker;
	const string fileName = "FileReaderWorkerDoWork.txt";
	const vector<string> content = {"First- +second", "third! #fourth", "fift$h", "sixth"};
	createFile(fileName, content);

	FileReaderWorker worker({ fileName });
	worker.doWork();
	auto result = worker.getOutput();
	EXPECT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "First- +second");
	EXPECT_EQ(result[1], "third! #fourth");
	EXPECT_EQ(result[2], "fift$h");
	EXPECT_EQ(result[3], "sixth");
}

TEST(FileWriterWorker, doWork)
{
	using namespace labWorker;
	const string fileName = "FileWriterWorkerDoWork.txt";
	const vector<string> content = { "First- +second", "third! #fourth", "fift$h", "sixth" };

	FileWriterWorker worker({ fileName });
	worker.setInput(content);
	worker.doWork();
	
	FileReaderWorker helper({ fileName });
	helper.doWork();
	auto result = helper.getOutput();
	EXPECT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "First- +second");
	EXPECT_EQ(result[1], "third! #fourth");
	EXPECT_EQ(result[2], "fift$h");
	EXPECT_EQ(result[3], "sixth");
}

TEST(FileDumpWorker, doWork)
{
	using namespace labWorker;
	const string fileName = "FileDumpWorkerDoWork.txt";
	const vector<string> content = { "First- +second", "third! #fourth", "fift$h", "sixth" };

	FileDumpWorker worker({ fileName });
	worker.setInput(content);
	worker.doWork();

	FileReaderWorker helper({ fileName });
	helper.doWork();
	auto redLines = helper.getOutput();
	EXPECT_EQ(redLines.size(), 4);
	EXPECT_EQ(redLines[0], worker.getOutput()[0]);
	EXPECT_EQ(redLines[1], worker.getOutput()[1]);
	EXPECT_EQ(redLines[2], worker.getOutput()[2]);
	EXPECT_EQ(redLines[3], worker.getOutput()[3]);
}

TEST(SortWorker, doWork)
{
	using namespace labWorker;
	const vector<string> content = {"1", "a", "zz", "z", "31", "23", "ba"};

	SortWorker worker({});
	worker.setInput(content);
	worker.doWork();
	auto result = worker.getOutput();

	EXPECT_EQ(result[0], "1");
	EXPECT_EQ(result[1], "23");
	EXPECT_EQ(result[2], "31");
	EXPECT_EQ(result[3], "a");
	EXPECT_EQ(result[4], "ba");
	EXPECT_EQ(result[5], "z");
	EXPECT_EQ(result[6], "zz");
}

TEST(ReplaceWorker, doWork)
{
	using namespace labWorker;
	const vector<string> content =
	{
		"Hello, programmer",
		"This test-unit is required to test ReplaceWorker",
		"There are no replaceable words",
		"Every \'programmer\' word will be replaced with word \'newbee\'"
	};

	const vector<string> args = { "programmer", "newbee" };
	ReplaceWorker worker(args);
	worker.setInput(content);
	worker.doWork();
	auto result = worker.getOutput();

	EXPECT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "Hello, newbee");
	EXPECT_EQ(result[1], content[1]);
	EXPECT_EQ(result[2], content[2]);
	EXPECT_EQ(result[3], "Every \'newbee\' word will be replaced with word \'newbee\'");
}

TEST(GrepWorker, doWork)
{
	using namespace labWorker;
	const vector<string> content =
	{
		"Hello, programmer",
		"This test-unit is required to test ReplaceWorker",
		"There are no replaceable words",
		"Every \'programmer\' word will be replaced with word \'newbee\'"
	};
	const vector<string> args = { "programmer" };
	GrepWorker worker(args);
	worker.setInput(content);
	worker.doWork();
	auto result = worker.getOutput();
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], content[0]);
	EXPECT_EQ(result[1], content[3]);
}