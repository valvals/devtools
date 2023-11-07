#include <io.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "html_parts.h"
#include "common_utils/common_utils.h"
#include "plotly_maker.h"

namespace{
// I will move it later
constexpr char kDivSizePart[] =R"(<div style="height:700px; width:700px;"
id="gd"></div>
<script>
)";

bool checkThatSizesAreTheSame(const std::vector<std::vector<double>> &values) {
    unsigned int size = 0;
    if(!values.empty()) {
        size = values[0].size();
    };
    for(unsigned int i=0;i<values.size();++i){

      if(values[i].size() != size)return false;
    }
    return true;
}

bool createStringHeatMapValues(const std::vector<std::vector<double>> &values,
                        std::string &str_values){
    if(!checkThatSizesAreTheSame(values))return false;
    if(!str_values.empty())str_values.clear();
    str_values.append(R"(var data = [{z: )");
    str_values.append(R"([)");
    for(unsigned int i=0;i<values.size();++i){
        str_values.append("[");
        for(unsigned int j=0;j<values[i].size();++j){
            str_values.append(std::to_string(values[i][j]));
            if(j!=values[i].size()-1)str_values.append(",");
        }
        str_values.append("]");
        if(i!=values.size()-1)str_values.append(",");
    }
    str_values.append(R"(],)");
    return true;
}

bool createStringLineChartValues(const std::vector<double> &values,
                                 std::string &str_values){
    if(!str_values.empty())str_values.clear();
    str_values = R"(var trace = {
x: [)";
    for(int i=0;i<values.size();++i){
        str_values.append(std::to_string(i));
        if(i!=values.size()-1)str_values.append(",");
    }
    str_values.append("], y: [");
    for(int j=0;j<values.size();++j){
    str_values.append(std::to_string(values[j]));
    if(j!=values.size()-1)str_values.append(",");
    }
    str_values.append("], mode: 'lines+markers'};var data = [trace];");
    return true;
}

} // namespace

namespace davis {

extern const char saveFolderName[] = "CreatedHtmls/";

bool createHtmlPageWithPlotlyJS(const std::vector<std::vector<double>> &values,
                                std::string &page)
{
    page = kCommonHeadPart;
    page.append(kDivSizePart);
    std::string str_values = "";
    if(!checkThatSizesAreTheSame(values))return false;
    createStringHeatMapValues(values, str_values);
    page.append(str_values);
    page.append(kColorMapDefaultPart);
    page.append(kCommonLastPart);
    return true;
}

bool showHeatMapInBrowser(const vector<vector<double>> &values, const std::string &title, const showSettings &settings){
    std::string page;
    if(!createHtmlPageWithPlotlyJS(values,page))return false;
    std::string pageName;
    struct stat sb;
    if (stat(saveFolderName, &sb) != 0)
        mkdir(saveFolderName);
    pageName.append("./").append(saveFolderName).append(title).append(".html");
    davis::saveStringToFile(pageName, page);
    openPlotlyHtml(pageName);
    return true;
}

bool showLineChartInBrowser(const vector<double> &values, const std::string &title, const showSettings &settings){
    std::string page = kCommonHeadPart;
    page.append(kDivSizePart);
    std::string str_values = "";
    createStringLineChartValues(values,str_values);
    page.append(str_values);
    page.append(kCommonLastPart);
    std::string pageName;
    struct stat sb;
    if (stat(saveFolderName, &sb) != 0)
        mkdir(saveFolderName);
    pageName.append("./").append(saveFolderName).append(title).append(".html");
    davis::saveStringToFile(pageName, page);
    davis::saveStringToFile(pageName,page);
    openPlotlyHtml(pageName);
    return true;
}

}; // namespace davis


