#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include "gtest/gtest.h"
#include "Builder.h"
#include "Compiler.h"
#include "Helper.h"

using namespace labHelper;
using namespace labBlock;
using namespace labWorker;
using namespace labCompiler;
using namespace std;

TEST(WorkflowBuilder, run_1)
{
	using namespace labBuilder;

	const vector<Block> chain =
	{
		Block(BlockType::READFILE, {"input.txt"}),
		Block(BlockType::GREP, {"hello"}),
		Block(BlockType::REPLACE, {"hello", "goodbye"}),
		Block(BlockType::WRITEFILE, {"output.txt"})
	};

	WorkflowBuilder builder(chain);
	EXPECT_NO_THROW(builder.run());
	auto result = builder.getBuiltCode();

	EXPECT_EQ(result.size(), chain.size());
	EXPECT_EQ(typeid(*result[0]), typeid(FileReaderWorker));
	EXPECT_EQ(typeid(*result[1]), typeid(GrepWorker));
	EXPECT_EQ(typeid(*result[2]), typeid(ReplaceWorker));
	EXPECT_EQ(typeid(*result[3]), typeid(FileWriterWorker));
}

TEST(WorkflowBuilder, run_2)
{
	using namespace labBuilder;
	const string fileName = "WorkflowBuilderRun.txt";
	const vector<string> content =
	{
		"desc",
		"id1 = readfile input.txt",
		"id2 = writefile output.txt",
		"csed",
		"id1 -> id2"
	};
	createFile(fileName, content);
	WorkflowCompiler compiler(fileName);
	compiler.run();

	WorkflowBuilder builder(compiler.getBlockChain());
	EXPECT_NO_THROW(builder.run());
	auto result = builder.getBuiltCode();
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(typeid(*result[0]), typeid(FileReaderWorker));
	EXPECT_EQ(typeid(*result[1]), typeid(FileWriterWorker));
}