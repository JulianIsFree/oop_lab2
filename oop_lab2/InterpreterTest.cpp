#include "pch.h"
#include "Interpreter.h"

void createInputFile(const std::string& fileName, const std::vector<std::string>& fileLines)
{
	std::ofstream fout(fileName);
	for(auto iter = fileLines.cbegin(); iter != fileLines.cend();++iter)
		fout << *iter << std::endl;
	fout.close();
}

TEST(ExecutionUnit, executeBlock)
{
	using namespace labInterpreter;
	using namespace labParser;
	ExecutionUnit unit;

	{
		std::vector<std::string> fileLines({ "A, B", "A, B, B", "B, A, B" });
		std::string fileName = "read_test.txt";
		createInputFile(fileName, fileLines);

		Block block;
		block.setBlockName(BlockName::READFILE);
		block.addContent(fileName);

		unit.executeBlock(block);
		const std::vector<std::string>& result = unit.getResult();

		EXPECT_EQ(fileLines.size(), result.size());
		for (size_t i = 0; i < fileLines.size(); ++i)
			EXPECT_EQ(fileLines[i], result[i]);
	}

	{
		std::vector<std::string> fileLinesToSort({ "e", "d", "c", "b", "a", "ab", "ac", "bc", "ed" });
		std::string fileName = "sort_test.txt";
		createInputFile(fileName, fileLinesToSort);

		Block block;
		block.setBlockName(BlockName::READFILE);
		block.setContent({ fileName });

		unit.executeBlock(block);
		block.setBlockName(BlockName::SORT);
		block.setContent({});

		unit.executeBlock(block);
		const std::vector<std::string>& result = unit.getResult();
		EXPECT_EQ(result.size(), fileLinesToSort.size());

		//{ "e", "d", "c", "b", "a", "bc", "ac", "ab", "ed" }
		EXPECT_EQ(result[0], "a");
		EXPECT_EQ(result[1], "ab");
		EXPECT_EQ(result[2], "ac");
		EXPECT_EQ(result[3], "b");
		EXPECT_EQ(result[4], "bc");
		EXPECT_EQ(result[5], "c");
		EXPECT_EQ(result[6], "d");
		EXPECT_EQ(result[7], "e");
		EXPECT_EQ(result[8], "ed");
	}

	{
		std::vector<std::string> fileLinesToGrep({ "abc abcd", "abcd", "abcd abc", "ab cd" });
		std::string fileName = "grep_test.txt";
		createInputFile(fileName, fileLinesToGrep);
		
		Block block;
		block.setBlockName(BlockName::READFILE);
		block.setContent({ fileName });

		//{ "abc abcd", "abcd", "abcd abc", "ab cd" }
		unit.executeBlock(block);
		block.setBlockName(BlockName::GREP);
		block.setContent({ "abcd" });

		unit.executeBlock(block);
		const std::vector<std::string>& result = unit.getResult();
		EXPECT_EQ(result.size(), 3);

		EXPECT_EQ(result[0], "abc abcd");
		EXPECT_EQ(result[1], "abcd");
		EXPECT_EQ(result[2], "abcd abc");
	}

	{
		std::vector<std::string> fileLinesToReplace({ "aa", "a", "b", "abcd" });
		std::string fileName = "replace_test.txt";
		createInputFile(fileName, fileLinesToReplace);

		Block block;
		block.setBlockName(BlockName::READFILE);
		block.setContent({ fileName });

		unit.executeBlock(block);
		block.setBlockName(BlockName::REPLACE);
		block.setContent({ "a", "aa" });

		unit.executeBlock(block);
		const std::vector<std::string>& result = unit.getResult();
		EXPECT_EQ(result.size(), fileLinesToReplace.size());

		EXPECT_EQ(result[0], "aaaa");
		EXPECT_EQ(result[1], "aa");
		EXPECT_EQ(result[2], "b");
		EXPECT_EQ(result[3], "aabcd");
	}
}

TEST(ExecutionUnit, run)
{
	using namespace labInterpreter;
	using namespace labParser;

	const std::string exeFileName = "text.txt";
	const std::vector<std::string> exeFileContent =
	{"desc",
	"code1 = readfile hello_text.txt",
	"code2 = sort",
	"code3 = grep Hello",
	"code4 = replace Hello bye",
	"code5 = dump result.txt",
	"csed",
	"code1 -> code3 -> code2 -> code4 -> code5"};
	createInputFile(exeFileName, exeFileContent);
	
	const std::string testFileName = "hello_text.txt";
	const std::vector<std::string> testFileContent =
	{"Hello, test programm!",
	"I wish you work correctly, because I hate debugging you.",
	"But don't worry, at the moment I complete this unit test I'll able to forget about you!",
	"I really can't hold myself from stop working on you",
	"Hello",
	"Hello a",
	"Hello b",
	"Hello hello"};

	createInputFile(testFileName, testFileContent);
	ExecutionUnit unit(exeFileName);
	ASSERT_NO_THROW(unit.run());

	const std::vector<std::string>& result = unit.getResult();
	EXPECT_EQ(result[0], "bye");
	EXPECT_EQ(result[1], "bye a");
	EXPECT_EQ(result[2], "bye b");
	EXPECT_EQ(result[3], "bye hello");
	EXPECT_EQ(result[4], "bye, test programm!");
}