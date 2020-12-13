#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "WorkflowProgramm.h"
#include "Worker.h"
#include "Helper.h"

using namespace labHelper;
using namespace labWorker;
using namespace std;

TEST(WorkflowProgramm, run_no_args)
{
	using namespace labProgramm;
	const string source = "WorkflowProgrammRunNoArgs.txt";
	const string inputFile = "WorkflowProgrammRunNoArgsInput.txt";
	const vector<string> inputFileContent = 
	{
		"It was a long way, but yes! I've completed this project times (3 times)",
		"And now it still isn't perfect, but much more better than in past",
		"Am I tired? Of course yes, why would I stop?"
	};
	const string outputFile = "WorkflowProgrammRunNoArgsOutputt.txt";
	const vector<string> sourceContent =
	{
		"desc",
		"id1 = readfile " + inputFile,
		"id2 = grep yes",
		"id3 = dump dump.txt",
		"id4 = replace yes no",
		"id5 = writefile " + outputFile,
		"csed",
		"id1 -> id2 -> id3 -> id4 -> id5"
	};

	createFile(inputFile, inputFileContent);
	createFile(source, sourceContent);

	char **argv = new char*[2];
	argv[0] = nullptr;

	argv[1] = new char[source.size() + 1];
	argv[1][source.size()] = '\0';
	strcpy(argv[1], source.c_str());

	WorkflowProgramm programm(2, argv);
	programm.run();

	FileReaderWorker helper({ outputFile });
	helper.doWork();
	auto result = helper.getOutput();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], "It was a long way, but no! I've completed this project times (3 times)");
	EXPECT_EQ(result[1], "Am I tired? Of course no, why would I stop?");

	delete argv[0];
	delete argv[1];
	delete argv;
}

TEST(WorkflowProgramm, run_args_i)
{
	using namespace labProgramm;
	const string source = "WorkflowProgrammRunArgsI.txt";
	const string inputFile = "WorkflowProgrammRunArgsIInput.txt";
	const vector<string> inputFileContent =
	{
		"It was a long way, but yes! I've completed this project times (3 times)",
		"And now it still isn't perfect, but much more better than in past",
		"Am I tired? Of course yes, why would I stop?"
	};
	const string outputFile = "WorkflowProgrammRunArgsIOutputt.txt";
	const vector<string> sourceContent =
	{
		"desc",
		"id2 = grep yes",
		"id3 = dump dump.txt",
		"id4 = replace yes no",
		"id5 = writefile " + outputFile,
		"csed",
		"id2 -> id3 -> id4 -> id5"
	};

	const string iFlag = "-i";
	int argc = 4;
	char **argv = new char*[argc];
	argv[0] = nullptr;
	argv[1] = new char[source.size() + 1];
	strcpy(argv[1], source.c_str());
	argv[2] = new char[iFlag.size() + 1];
	strcpy(argv[2], iFlag.c_str());
	argv[3] = new char[inputFile.size() + 1];
	strcpy(argv[3], inputFile.c_str());

	createFile(inputFile, inputFileContent);
	createFile(source, sourceContent);

	WorkflowProgramm programm(argc, argv);
	programm.run();

	FileReaderWorker helper({ outputFile });
	helper.doWork();
	auto result = helper.getOutput();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], "It was a long way, but no! I've completed this project times (3 times)");
	EXPECT_EQ(result[1], "Am I tired? Of course no, why would I stop?");
	delete argv[1];
	delete argv[2];
	delete argv[3];
	delete argv;
}

TEST(WorkflowProgramm, run_args_o)
{
	using namespace labProgramm;
	const string source = "WorkflowProgrammRunO.txt";
	const string inputFile = "WorkflowProgrammRunOInput.txt";
	const vector<string> inputFileContent =
	{
		"It was a long way, but yes! I've completed this project times (3 times)",
		"And now it still isn't perfect, but much more better than in past",
		"Am I tired? Of course yes, why would I stop?"
	};
	const string outputFile = "WorkflowProgrammRunOOutputt.txt";
	const vector<string> sourceContent =
	{
		"desc",
		"id1 = readfile " + inputFile,
		"id2 = grep yes",
		"id3 = dump dump.txt",
		"id4 = replace yes no",
		"csed",
		"id1 -> id2 -> id3 -> id4"
	};

	createFile(inputFile, inputFileContent);
	createFile(source, sourceContent);

	const string oFlag = "-o";
	int argc = 4;
	char **argv = new char*[argc];
	argv[0] = nullptr;
	argv[1] = new char[source.size() + 1];
	strcpy(argv[1], source.c_str());
	argv[2] = new char[oFlag.size() + 1];
	strcpy(argv[2], oFlag.c_str());
	argv[3] = new char[outputFile.size() + 1];
	strcpy(argv[3], outputFile.c_str());

	WorkflowProgramm programm(argc, argv);
	programm.run();

	FileReaderWorker helper({ outputFile });
	helper.doWork();
	auto result = helper.getOutput();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], "It was a long way, but no! I've completed this project times (3 times)");
	EXPECT_EQ(result[1], "Am I tired? Of course no, why would I stop?");

	delete argv[0];
	delete argv[1];
	delete argv[2];
	delete argv;
}

TEST(WorkflowProgramm, run_args_io)
{
	using namespace labProgramm;
	const string source = "WorkflowProgrammRunIO.txt";
	const string inputFile = "WorkflowProgrammRunIOnput.txt";
	const vector<string> inputFileContent =
	{
		"It was a long way, but yes! I've completed this project times (3 times)",
		"And now it still isn't perfect, but much more better than in past",
		"Am I tired? Of course yes, why would I stop?"
	};
	const string outputFile = "WorkflowProgrammRunIOOutputt.txt";
	const vector<string> sourceContent =
	{
		"desc",
		"id2 = grep yes",
		"id3 = dump dump.txt",
		"id4 = replace yes no",
		"csed",
		"id2 -> id3 -> id4"
	};

	createFile(inputFile, inputFileContent);
	createFile(source, sourceContent);

	const string oFlag = "-o";
	const string iFlag = "-i";
	int argc = 6;
	char **argv = new char*[argc];
	argv[0] = nullptr;
	argv[1] = new char[source.size() + 1];
	strcpy(argv[1], source.c_str());
	argv[2] = new char[oFlag.size() + 1];
	strcpy(argv[2], oFlag.c_str());
	argv[3] = new char[outputFile.size() + 1];
	strcpy(argv[3], outputFile.c_str());
	argv[4] = new char[iFlag.size() + 1];
	strcpy(argv[4], iFlag.c_str());
	argv[5] = new char[inputFile.size() + 1];
	strcpy(argv[5], inputFile.c_str());

	WorkflowProgramm programm(argc, argv);
	programm.run();

	FileReaderWorker helper({ outputFile });
	helper.doWork();
	auto result = helper.getOutput();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], "It was a long way, but no! I've completed this project times (3 times)");
	EXPECT_EQ(result[1], "Am I tired? Of course no, why would I stop?");

	delete argv[0];
	delete argv[1];
	delete argv[2];
	delete argv[3];
	delete argv[4];
	delete argv[5];
	delete argv;
}