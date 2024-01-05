#ifndef COMMON_UTILS_COMMON_UTILS_H_
#define COMMON_UTILS_COMMON_UTILS_H_
#include <string>
#include <vector>

namespace davis {

using std::string;
using std::vector;

std::string getCurrentPath();

bool isPlotlyScriptExists();

bool saveStringToFile(const string& file_name,
                      const string& data);

void mayBeCreateJsWorkingFolder();

void sleepMs(unsigned long milisec);

void openPlotlyHtml(const string& file_name);

bool getDataFromFile(const string& path, string& result);

std::vector<std::string> split(const std::string& target, char c);

bool readMatrix(vector<vector<double>>& outMatrix, const string& path, char dlmtr);


// Now it doesn't work.
bool deleteFolder(const char* fname);

}; // namespace davis

#endif // COMMON_UTILS_COMMON_UTILS_H_
