#include <vector>
//#include <string>
#include "gtest/gtest.h"
#include "Builder.h"

using namespace labBlock;
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
}

TEST(WorkflowBuilder, run_2)
{

}