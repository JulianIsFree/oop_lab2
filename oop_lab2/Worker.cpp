#include <fstream>
#include <algorithm>
#include <locale>

#include "Helper.h"
#include "Exception.h"
#include "Worker.h"

using namespace labException;
using namespace labHelper;
using namespace std;

void labWorker::FileReaderWorker::doWork()
{
	ifstream fin(args[0]);
	if (!fin.is_open())
		throw FileNotOpened(args[0]);

	string line;
	while (std::getline(fin, line))
		output.push_back(line);

	fin.close();
}

void labWorker::FileWriterWorker::doWork()
{
	ofstream fout(args[0]);
	if (!fout.is_open())
		throw FileNotOpened(args[0]);

	for (auto iter = input.cbegin(); iter != input.cend(); ++iter)
		fout << *iter << endl;

	fout.close();
}

void labWorker::FileDumpWorker::doWork()
{
	output = input;
	FileWriterWorker::doWork();
}

void labWorker::GrepWorker::doWork()
{
	string word = args[0];
	for (auto iter = input.cbegin(); iter != input.cend(); ++iter)
	{
		if (iter->find(word) != iter->npos)
			output.push_back(*iter);
	}
}

void labWorker::SortWorker::doWork()
{
	output = input;
	sort(output.begin(), output.end());
}

void labWorker::ReplaceWorker::doWork()
{
	const std::string replaceable = args[0];
	const std::string replacement = args[1];
	const size_t len = replaceable.length();

	for (auto iter = input.cbegin(); iter != input.cend(); ++iter)
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
		output.push_back(resultStr);
	}
}
