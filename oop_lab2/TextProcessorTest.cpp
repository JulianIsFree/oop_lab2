#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <fstream>
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "TextProcessor.h"
#include "Helper.h"
using namespace std;
using namespace labHelper;

TEST(TextProcessor, getNextWord)
{
	using namespace labTextProcessor;
	const string fileName = "TextProcessorGetNextWordTest.txt";
	const vector<string> content = {"word1 = word2", "word3 = word4" };
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	EXPECT_EQ(processor.getNextWord(), "word1");
	EXPECT_EQ(processor.getNextWord(), "=");
	EXPECT_EQ(processor.getNextWord(), "word2");
	EXPECT_EQ(processor.getNextWord(), "word3");
	EXPECT_EQ(processor.getNextWord(), "=");
	EXPECT_EQ(processor.getNextWord(), "word4");
}

TEST(TextProcessor, isCorrect)
{
	using namespace labTextProcessor;
	const string fileName = "TextProcessorIsCorrect.txt";
	const vector<string> content = { "word1234", "word_", "-word", "+word", "word("};
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	EXPECT_EQ(processor.isCorrect(content[0]), true);
	EXPECT_EQ(processor.isCorrect(content[1]), false);
	EXPECT_EQ(processor.isCorrect(content[2]), true);
	EXPECT_EQ(processor.isCorrect(content[3]), false);
	EXPECT_EQ(processor.isCorrect(content[4]), false);
}

TEST(TextProcessor, hasNext)
{
	using namespace labTextProcessor;
	const string fileName = "TextProcessorHasNext.txt";
	const vector<string> content = { "1" };
	createFile(fileName, content);

	WorkflowTextProcessor processor(fileName);
	EXPECT_EQ(processor.hasNext(), true);
	processor.getNextWord();
	EXPECT_EQ(processor.hasNext(), false);
}
