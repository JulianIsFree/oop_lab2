#pragma once
#include <vector>
#include <map>
#include "Parser.h"

namespace labInterpreter
{
	using namespace labParser;

	class Interpreter
	{
	protected:
		Parser parser;
	public:
		Interpreter(const std::string& fileName) : parser(Parser(fileName)) {};
		Interpreter() {};
		void run() {};
	};

	class ExecutionUnit : Interpreter
	{
	private:
		std::vector<std::string> blockExecutionInput;
		std::vector<std::string> blockExecutionResult;

		void writefile(const Block& block);
		void readfile(const Block& block);
		void dump(const Block& block);
		void grep(const Block& block);
		void replace(const Block& block);
		void sort();
	public:
		ExecutionUnit(const std::string& fileName) : Interpreter(fileName) {};
		ExecutionUnit() {};
		void executeBlock(const Block& block);
		void run();
		void resetState();
		const std::vector<std::string>& getResult();
	};
};