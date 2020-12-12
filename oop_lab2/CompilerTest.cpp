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

	vector<Block> chain = compiler.getBlockChain();
	EXPECT_EQ(chain.size(), 5);
	EXPECT_EQ(compiler.getBlocks().size(), 4);

	EXPECT_EQ(chain[0].getType(), BlockType::READFILE);
	EXPECT_EQ(chain[0].getContent().size(), 1);
	EXPECT_EQ(chain[0].getContent()[0], "input.txt");
	
	EXPECT_EQ(chain[1].getType(), BlockType::GREP);
	EXPECT_EQ(chain[1].getContent().size(), 1);
	EXPECT_EQ(chain[1].getContent()[0], "hello");
	
	EXPECT_EQ(chain[2].getType(), BlockType::GREP);
	EXPECT_EQ(chain[2].getContent().size(), 1);
	EXPECT_EQ(chain[2].getContent()[0], "hello");
	
	EXPECT_EQ(chain[3].getType(), BlockType::DUMP);
	EXPECT_EQ(chain[3].getContent().size(), 1);
	EXPECT_EQ(chain[3].getContent()[0], "dump.txt");

	EXPECT_EQ(chain[4].getType(), BlockType::WRITEFILE);
	EXPECT_EQ(chain[4].getContent().size(), 1);
	EXPECT_EQ(chain[4].getContent()[0], "output.txt");
}

TEST(WorkflowCompiler, run)
{
	using namespace labCompiler;
	using namespace labException;

	const string fileName = "WorkflowCompilerRun.txt";
	vector<string> content =
	{
		"desc",
		"name1 = readfile input.txt",
		"name2 = grep hello",
		"name3 = dump dump.txt",
		"name4 = readfile input.txt",
		"csed",
		"name1 -> name2 -> name2 -> name3 -> name4"
	};
	createFile(fileName, content);

	WorkflowCompiler compiler(fileName);
	EXPECT_THROW(compiler.run(), BlockSequenceIsNotAllowedException("dump", "readfile"));
}