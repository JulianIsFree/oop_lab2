#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "Compiler.h"
#include "Exception.h"
#include "Block.h"
#include "Helper.h"

using namespace labBlock;
using namespace labHelper;
using namespace std;

TEST(WorkflowCompiler, run)
{
	using namespace labCompiler;

	const string fileName = "WorkflowCompilerRun.txt";
	vector<string> content = 
	{
		"desc", 
		"name1 = readfile input.txt", 
		"name2 = grep hello", 
		"name3 = dump dump.txt", 
		"name4 = writefile output.txt", 
		"csed", 
		"name1 -> name2 -> name2 -> name3 -> name4"
	};
	createFile(fileName, content);

	WorkflowCompiler compiler(fileName);
	EXPECT_NO_THROW(compiler.run());

	list<Block> chain = compiler.getBlockChain();
	EXPECT_EQ(chain.size(), 5);
	EXPECT_EQ(compiler.getBlocks().size(), 4);

	auto iter = chain.begin();
	EXPECT_EQ(iter->getType(), BlockType::READFILE);
	EXPECT_EQ(iter->getContent().size(), 1);
	EXPECT_EQ(iter->getContent()[0], "input.txt");
	++iter;

	EXPECT_EQ(iter->getType(), BlockType::GREP);
	EXPECT_EQ(iter->getContent().size(), 1);
	EXPECT_EQ(iter->getContent()[0], "hello");
	++iter;

	EXPECT_EQ(iter->getType(), BlockType::GREP);
	EXPECT_EQ(iter->getContent().size(), 1);
	EXPECT_EQ(iter->getContent()[0], "hello");
	++iter;

	EXPECT_EQ(iter->getType(), BlockType::DUMP);
	EXPECT_EQ(iter->getContent().size(), 1);
	EXPECT_EQ(iter->getContent()[0], "dump.txt");
	++iter;

	EXPECT_EQ(iter->getType(), BlockType::WRITEFILE);
	EXPECT_EQ(iter->getContent().size(), 1);
	EXPECT_EQ(iter->getContent()[0], "output.txt");
}

//TEST(WorkflowCompiler, run_throws1)
//{
//	using namespace labCompiler;
//	using namespace labException;
//
//	const string fileName = "WorkflowCompilerRun.txt";
//	vector<string> content =
//	{
//		"desc",
//		"name1 = readfile input.txt",
//		"name2 = grep hello",
//		"name3 = dump dump.txt",
//		"name4 = readfile input.txt", //readfile after dump isn't allowed
//		"csed",
//		"name1 -> name2 -> name2 -> name3 -> name4"
//	};
//	createFile(fileName, content);
//
//	WorkflowCompiler compiler(fileName);
//	EXPECT_ANY_THROW(compiler.run());
//}
