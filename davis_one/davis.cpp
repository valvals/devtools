
//This file was generated by davis_one_maker.py script.
//Use davis_files.txt and special comments to change generated davis.h and davis.cpp

#include "davis.h"

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <sys/stat.h>
#include <vector>
namespace dvs {
const char kAppName[] = "davis";
const char kOutFolderName[] = "davis_htmls/";
const char kPlotlyJsWorkPath[] = "./davis_htmls/plotly-2.27.0.min.js";
const char kPlotlyJsName[] = "plotly-2.27.0.min.js";
const char kPlotlyJsResourcePath[] = "plotly_maker/plotly-2.27.0.min.js";

} // namespace dvs end

namespace dv {

Configurator& config() {
  return Configurator::getInstance();
};



} // namespace dv end
namespace dvs {
// *INDENT-OFF*
    const char kDivSizePart[] = R"(<div style = "display: flex;
  align-items:center;height:100%; width:100%;background:#dddfd4;
  justify-content: center;"><div style="height:95%; aspect-ratio: 1/1;"
id="gd"></div></div>
<script>
)";

    const char kCommonHeadPart[] = R"(<head>
<script src="./plotly-2.27.0.min.js" charset="utf-8"></script>
</head>
<body>)";

    const char kColorMapDefaultPart[] = R"(
  colorscale: [
    ['0.0', 'rgb(165,0,38)'],
    ['0.111111111111', 'rgb(215,48,39)'],
    ['0.222222222222', 'rgb(244,109,67)'],
    ['0.333333333333', 'rgb(253,174,97)'],
    ['0.444444444444', 'rgb(254,224,144)'],
    ['0.555555555556', 'rgb(224,243,248)'],
    ['0.666666666667', 'rgb(171,217,233)'],
    ['0.777777777778', 'rgb(116,173,209)'],
    ['0.888888888889', 'rgb(69,117,180)'],
    ['1.0', 'rgb(49,54,149)']
  ],)";


    const char kColorMapSunnyPart[] = R"(
  colorscale: [
    ['0.0', 'rgb(250, 134, 7)'],
    ['0.2', 'rgb(250, 150, 27)'],
    ['0.4', 'rgb(252, 176, 51)'],
    ['0.6', 'rgb(254, 204, 81)'],
    ['0.8', 'rgb(255, 228, 105)'],
    ['1.0', 'rgb(255, 245, 123)']
  ],)";

    const char kColorMapGlamourPart[] = R"(
  colorscale: [
    ['0.0', 'rgb(17,63,93)'],
    ['0.2', 'rgb(88,80,143)'],
    ['0.4', 'rgb(182,79,145)'],
    ['0.6', 'rgb(245,97,94)'],
    ['0.8', 'rgb(248,165,0)'],
    ['1.0', 'rgb(235,248,0)']
  ],)";

    const char kColorMapThermalPart[] = R"(
  colorscale: [
    ['0.0', 'rgb(0,0,5)'],
    ['0.2', 'rgb(12,0,44)'],
    ['0.4', 'rgb(41,0,148)'],
    ['0.6', 'rgb(196,0,123)'],
    ['0.8', 'rgb(230,61,63)'],
    ['1.0', 'rgb(255,213,0)']
  ],)";


    const char kColorMapGrayscalePart[] = R"(
  colorscale: [
    ['0.0', 'rgb(0,0,0)'],
    ['1.0', 'rgb(255, 255, 255)']
  ],)";

    const char kHeatMapTypePart[] = R"(
type: 'heatmap',
hovertemplate: 'x:%{x} <br>y:%{y} <br>val:%{z:.}<extra></extra>'
}];)";

    const char kSurfaceTypePart[]=R"(
type: 'surface',
hovertemplate: 'x:%{x} <br>y:%{y} <br>z:%{z:.}<extra></extra>'
}];)";

    const char kCommonLastPart[] = R"(
var layout;
var config = {
  editable: true,
  showLink: true,
  plotlyServerURL: "https://chart-studio.plotly.com"
};
Plotly.newPlot('gd', data, layout, config);
</script>
</body>)";

// *INDENT-ON*

} // namespace dvs end

namespace dvs {
using std::string;

#ifdef _WIN32
  #include <direct.h>
  #include <windows.h>
  #define getcwd _getcwd // stupid MSFT "deprecation" warning
#elif __linux__
  #include <unistd.h>
#endif

inline bool is_file_exists(const string& file_name) {
  std::ifstream file(file_name.c_str());
  if (!file) {
    return false;
  }
  return true;
}

void openFileBySystem(const string& file_name) {
  string command;
#ifdef _WIN32
  command = "start ";
#elif __APPLE__
  command = "open ";
#elif __linux__
  command = "xdg-open ";
#else
#error "Unknown compiler"
#endif
  command.append(file_name);
  system(command.c_str());
}

string getCurrentPath() {
#if defined (_WIN32) || (__linux__)
  char buffer[1024];
  char* answer = getcwd(buffer, sizeof(buffer));
  string s_cwd;
  if (answer) {
    s_cwd = answer;
  }
  return s_cwd;
#elif __APPLE__
  //TODO macos get current path implementation
  return "";
#endif
}

bool isPlotlyScriptExists() {
  return is_file_exists(kPlotlyJsWorkPath);
}

bool saveStringToFile(const string& file_name,
                      const string& data) {
  std::ofstream out(file_name);
  if (out.is_open()) {
    out << data.c_str();
    out.close();
    return true;
  }
  return false;
}


void openPlotlyHtml(const string& file_name) {
  openFileBySystem(file_name);
}

void sleepMs(unsigned long milisec) {
#ifdef _WIN32
  Sleep(milisec);
#elif __linux__
  usleep(milisec * 1000);
#endif
}

void mayBeCreateJsWorkingFolder() {
  struct stat sb;
  if (stat(kOutFolderName, &sb) != 0) {
#ifdef _WIN32
    _mkdir(kOutFolderName);
#elif __linux__
    mode_t mode = 0755;
    mkdir(kOutFolderName, mode);
#endif
  }
}

bool deleteFolder(const char* fname) {
  struct stat sb;
  if (stat(fname, &sb) == 0) {
    //rmdir(fname);
    return true;
  } else {
    return false;
  }
}

bool getDataFromFile(const string& path, string& result) {

  //TODO different scenarious and sanitizing
  if (!is_file_exists(path)) {
    return false;
  }
  if (!result.empty()) {
    result.clear();
  }
  std::fstream file;
  file.open(path, std::ios::in);
  if (file.is_open()) {
    string temp;
    while (std::getline(file, temp)) {
      result.append(temp).append(";");
    }
  } else {
    return false;
  }
  return true;
}

bool readMatrix(vector<vector<double>>& outMatrix, const std::string& path, char dlmtr) {
  outMatrix.clear();
  std::ifstream ifs;
  std::string str;
  ifs.open(path, std::ios::in);
  if (ifs) {
    while (!ifs.eof()) {
      std::getline(ifs, str);
      if (str.size() == 0) //if exist empty line
        continue;
      std::vector<std::string> parts = split(str, dlmtr);
      vector<double> doubleLine;
      doubleLine.resize(parts.size());
      for (size_t i = 0; i < parts.size(); ++i) {
        doubleLine[i] = std::stod(parts.at(i));
      }
      outMatrix.push_back(doubleLine);
    }
    ifs.close();
    return true;
  } else {
    std:: cout << "Unable to open file to read: " << path << std::endl;
    return false;
  }
}

vector<string> split(const string& target, char c) {
  std::string temp;
  std::stringstream stringstream { target };
  std::vector<std::string> result;
  while (std::getline(stringstream, temp, c)) {
    result.push_back(temp);
  }

  return result;
}

bool make_string(const string& src,
                 const vector<string>& args,
                 string& out) {
  if (!out.empty()) {
    out.clear();
  }
  if (args.empty()) {
    return false;
  }
  vector<vector<size_t>> road_map;
  size_t reserve_size = 0;
  size_t pos = 0;
  while (pos < src.size()) {
    size_t new_pos = src.find('%', pos);
    if (new_pos == string::npos) {
      //out.append(src.substr(pos, src.size() - pos));
      reserve_size += (src.size() - pos);
      road_map.push_back({pos, src.size() - pos});
      break;
    };
    std::string arg_index;
    size_t temp_pos = 0;
    temp_pos = new_pos;
    while (temp_pos < src.size() && isdigit(src[++temp_pos])) {
      arg_index += src[temp_pos];
    }
    //string part = src.substr(pos, new_pos - pos);
    road_map.push_back({pos, new_pos - pos});
    reserve_size += (new_pos - pos);
    if (!arg_index.empty()) {
      size_t index = std::stol(arg_index);
      if (index > 0 && index <= args.size()) {
        //part.append(args[index - 1]);
        reserve_size += args[index - 1].size();
        road_map.push_back({index - 1});
      } else {
        //TODO return false or continue
      }
    } else {
      //part.append("%");
      road_map.push_back({UINT_MAX});
      ++reserve_size;
    }
    //out.append(part);
    pos = temp_pos;
  }
  // create out according on the road map
  out.reserve(reserve_size);
  for (int i = 0; i < road_map.size(); ++i) {
    auto size = road_map[i].size();
    if (size == 2) {
      out.append(src.substr(road_map[i][0], road_map[i][1]));
    } else if (size == 1) {
      if (road_map[i][0] == UINT_MAX) {
        out.append("%");
      } else {
        out.append(args[road_map[i][0]]);
      }
    }
  }
  //std::cout<<"\n\n"<<reserve_size<<"<-->"<<out.size();
  return true;
}

} // namespace dvs end

namespace dvs {



bool checkThatSizesAreTheSame(const vector<vector<double>>& values) {
  size_t size = 0;
  if (!values.empty()) {
    size = values[0].size();
  };
  for (size_t i = 0; i < values.size(); ++i) {

    if (values[i].size() != size)
      return false;
  }
  return true;
}

bool createStringHeatMapValues(const vector<vector<double>>& values,
                               string& str_values) {
  if (!checkThatSizesAreTheSame(values))
    return false;
  if (!str_values.empty())
    str_values.clear();
  str_values.append(R"(var data = [{z: )");
  str_values.append(R"([)");
  for (size_t i = 0; i < values.size(); ++i) {
    str_values.append("[");
    for (size_t j = 0; j < values[i].size(); ++j) {
      str_values.append(std::to_string(values[i][j]));
      if (j != values[i].size() - 1) {
        str_values.append(",");
      }
    }
    str_values.append("]");
    if (i != values.size() - 1) {
      str_values.append(",");
    }
  }
  str_values.append(R"(],)");
  return true;
}


bool createStringLineChartValues(const vector<double>& values,
                                 string& str_values) {
  if (!str_values.empty()) {
    str_values.clear();
  }
  str_values = R"(var trace = {x: [)";
  for (size_t i = 0; i < values.size(); ++i) {
    str_values.append(std::to_string(i));
    if (i != values.size() - 1) {
      str_values.append(",");
    }
  }
  str_values.append("], y: [");
  for (size_t j = 0; j < values.size(); ++j) {
    str_values.append(std::to_string(values[j]));
    if (j != values.size() - 1) {
      str_values.append(",");
    }
  }
  str_values.append("], mode: 'lines+markers', hovertemplate: 'x:%{x}, y:%{y:.} <extra></extra>' };var data = [trace];");
  return true;
}

inline bool heatmap_and_surface(const vector<vector<double>>& values,
                                const string& title,
                                const dv::conf_visualizationTypes& type) {
  string page;
  if (!createHtmlPageWithPlotlyJS(values, page, type)) {
    return false;
  }
  string pageName;
  mayBeCreateJsWorkingFolder();
  pageName.append("./").append(kOutFolderName).append(title).append(".html");
  saveStringToFile(pageName, page);
  openPlotlyHtml(pageName);
  return true;// TODO handle different exceptions
};

bool getMatrixValuesFromString(const string& in_values,
                               vector<vector<double>>& out_values) {
  istringstream f_lines(in_values);
  string lines;
  while (std::getline(f_lines, lines, ';')) {
    vector<double>vals;
    istringstream f_values(lines);
    string str_value;
    while (std::getline(f_values, str_value, ',')) {
      vals.push_back(std::stod(str_value));
    }
    out_values.push_back(vals);
  }
  return true;
};

bool createHtmlPageWithPlotlyJS(const std::vector<std::vector<double>>& values,
                                string& page,
                                const dv::conf_visualizationTypes& type) {
  page = kCommonHeadPart;
  page.append(kDivSizePart);
  string str_values = "";
  if (!checkThatSizesAreTheSame(values)) {
    return false;
  }
  createStringHeatMapValues(values, str_values);
  page.append(str_values);
  dv::conf_colorscales clrScale;
  if (type == dv::conf_visualizationTypes::HEATMAP)
    clrScale = dv::config().heatmap.colorSc;
  else if (type == dv::conf_visualizationTypes::SURFACE)
    clrScale = dv::config().surf.colorSc;
  switch (clrScale) {
    case dv::conf_colorscales::DEFAULT:
      page.append(kColorMapDefaultPart);
      break;
    case dv::conf_colorscales::SUNNY:
      page.append(kColorMapSunnyPart);
      break;
    case dv::conf_colorscales::GLAMOUR:
      page.append(kColorMapGlamourPart);
      break;
    case dv::conf_colorscales::THERMAL:
      page.append(kColorMapThermalPart);
      break;
    case dv::conf_colorscales::GRAYSCALE:
      page.append(kColorMapGrayscalePart);
      break;
  }
  switch (type) {
    case dv::conf_visualizationTypes::HEATMAP:
      page.append(kHeatMapTypePart);
      break;
    case dv::conf_visualizationTypes::SURFACE:
      page.append(kSurfaceTypePart);
      break;
    default:
      break;
  }
  page.append(kCommonLastPart);
  return true;
}

bool showHeatMapInBrowser(const vector<vector<double>>& values,
                          const string& title) {
  return heatmap_and_surface(values, title, dv::conf_visualizationTypes::HEATMAP);
}

bool showHeatMapInBrowser(const string& values,
                          const string& title) {
  vector<vector<double>>heat_map_values;
  getMatrixValuesFromString(values, heat_map_values);
  showHeatMapInBrowser(heat_map_values, title);
  return true;
};

bool showLineChartInBrowser(const vector<double>& values,
                            const string& title) {
  string page = kCommonHeadPart;
  page.append(kDivSizePart);
  string str_values = "";
  createStringLineChartValues(values, str_values);
  page.append(str_values);
  page.append(kCommonLastPart);
  string pageName;
  mayBeCreateJsWorkingFolder();
  pageName.append("./").append(kOutFolderName).append(title).append(".html");
  saveStringToFile(pageName, page);
  openPlotlyHtml(pageName);
  return true;
}

bool showLineChartInBrowser(const string& values,
                            const string& title) {
  vector<double>vals;
  istringstream f(values);
  string s;
  while (std::getline(f, s, ',')) {
    vals.push_back(std::stod(s));
  }
  showLineChartInBrowser(vals, title);
  return true;
};

bool showSurfaceInBrowser(const vector<vector<double>>& values,
                          const string& title) {
  return heatmap_and_surface(values, title, dv::conf_visualizationTypes::SURFACE);
}

bool showSurfaceInBrowser(const string& values,
                          const string& title) {
  vector<vector<double>>surface_values;
  getMatrixValuesFromString(values, surface_values);
  showSurfaceInBrowser(surface_values, title);
  return true;
}


} // namespace dvs end

