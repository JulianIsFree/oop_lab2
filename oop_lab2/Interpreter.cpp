#include "pch.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Interpreter.h"
#include "Helper.h"
#include "Exceptions.h"

using namespace labExceptions;
using namespace labInterpreter;

void ExecutionUnit::writefile(const Block & block)
{
	const std::string fileName = block.getContent()[0];
	std::ofstream fout(fileName);

	if(!fout.is_open())
		throw FileInputError(codeHelper::fileNotOpened(fileName));
	
	blockExecutionInput = blockExecutionResult;
	blockExecutionResult.clear();

	for (auto iter = blockExecutionInput.cbegin(); iter != blockExecutionInput.cend(); ++iter)
		fout << *iter << std::endl;
}

void labInterpreter::ExecutionUnit::readfile(const Block & block)
{
	const std::string fileName = block.getContent()[0];
	std::ifstream fin(fileName);
	
	if (!fin.is_open())
		throw FileInputError(codeHelper::fileNotOpened(fileName));
	
	blockExecutionInput.clear();
	blockExecutionResult.clear();
	std::string line;

	while (std::getline(fin, line))
		blockExecutionResult.push_back(line);
}

void labInterpreter::ExecutionUnit::dump(const Block & block)
{
	const std::string fileName = block.getContent()[0];
	std::ofstream fout(fileName);

	if (!fout.is_open())
		throw FileInputError(codeHelper::fileNotOpened(fileName));

	blockExecutionInput = blockExecutionResult;
	
	for (auto iter = blockExecutionInput.cbegin(); iter != blockExecutionInput.cend(); ++iter)
		fout << *iter << std::endl;
}

void labInterpreter::ExecutionUnit::replace(const Block & block)
{
	const std::string replaceable = block.getContent()[0];
	const std::string replacement = block.getContent()[1];
	const size_t len = replaceable.length();
	blockExecutionInput = blockExecutionResult;
	blockExecutionResult.clear();

	for (auto iter = blockExecutionInput.cbegin(); iter != blockExecutionInput.cend(); ++iter)
	{
		std::string str = *iter;
		size_t index;
		std::string resultStr = "";
		while ((index = str.find(replaceable)) != str.npos)
		{
			std::string substr = str.substr(0, index + len);
			substr.replace(index, len, replacement);
			resultStr += substr;
			str.erase(0, index + replaceable.length());
		}

		resultStr += str;
		blockExecutionResult.push_back(resultStr);
	}
}

void labInterpreter::ExecutionUnit::sort()
{
	blockExecutionInput = blockExecutionResult;
	std::sort(blockExecutionResult.begin(), blockExecutionResult.end());
}

void labInterpreter::ExecutionUnit::grep(const Block & block)
{
	blockExecutionInput = blockExecutionResult;
	blockExecutionResult.clear();
	const std::string strToFind = block.getContent()[0];
	for (auto iter = blockExecutionInput.cbegin(); iter != blockExecutionInput.cend(); ++iter)
	{
		if (iter->find(strToFind) != iter->npos)
			blockExecutionResult.push_back(*iter);
	}
}

void labInterpreter::ExecutionUnit::executeBlock(const Block & block)
{
	switch (block.getBlockName())
	{
	case BlockName::DUMP:
		dump(block);
		break;
	case BlockName::READFILE:
		readfile(block);
		break;
	case BlockName::WRITEFILE:
		writefile(block);
		break;
	case BlockName::REPLACE:
		replace(block);
		break;
	case BlockName::GREP:
		grep(block);
		break;
	case BlockName::SORT:
		sort();
		break;
	default:
		throw BadBlock(block2str(block.getBlockName()) + ", can't execute.");
	}
}

void labInterpreter::ExecutionUnit::run()
{
	resetState();
	try
	{
		parser.run();
	}
	catch (Exception e)
	{
		std::cout << "Parsing failed" << std::endl;
		throw e;
	}
	
	const std::vector<Block>& sequence = parser.getBlockSequence();
	const std::map<std::string, Block>& blockTable = parser.getBlockTable();
	
	try 
	{
		for (auto iter = sequence.cbegin(); iter != sequence.cend(); ++iter)
			executeBlock(*iter);
	}
	catch (Exception e)
	{
		resetState();
		e.printMessage();
		throw e;
	}

}

void labInterpreter::ExecutionUnit::resetState()
{
	blockExecutionInput.clear();
	blockExecutionResult.clear();
}

const std::vector<std::string>& labInterpreter::ExecutionUnit::getResult()
{
	return blockExecutionResult;
}

int main()
{

	return 0;
}