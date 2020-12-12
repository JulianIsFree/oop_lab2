#include <fstream>
#include "Helper.h"
using namespace std;
void labHelper::createFile(const std::string & fileName, const std::vector<std::string>& content)
{
	ofstream fout(fileName);
	for (auto iter = content.cbegin(); iter != content.cend(); ++iter)
		fout << *iter << endl;
	fout.close();
}