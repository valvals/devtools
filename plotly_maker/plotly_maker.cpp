//#START_GRAB_TO_INCLUDES_LIST
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
//#STOP_GRAB_TO_INCLUDES_LIST

#include "html_parts.h"
#include "common_utils/common_utils.h"
#include "common_utils/common_constants.h"
#include "plotly_maker.h"


namespace dvs {
//#START_GRAB_TO_DVS_NAMESPACE



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
  vector<string> args(MAX_ELEMENT,"");
  string str_values = "";
  if (!checkThatSizesAreTheSame(values)) {
    return false;
  }
  createStringHeatMapValues(values, str_values);
  args[VALUES]=str_values;
  dv::conf_colorscales clrScale;
  if (type == dv::conf_visualizationTypes::HEATMAP)
    clrScale = dv::config().heatmap.colorSc;
  else if (type == dv::conf_visualizationTypes::SURFACE)
    clrScale = dv::config().surf.colorSc;
  switch (clrScale) {
    case dv::conf_colorscales::DEFAULT:
      args[COLOR_MAP]=kColorMapDefaultPart;
      break;
    case dv::conf_colorscales::SUNNY:
      args[COLOR_MAP]=kColorMapSunnyPart;
      break;
    case dv::conf_colorscales::GLAMOUR:
      args[COLOR_MAP]=kColorMapGlamourPart;
      break;
    case dv::conf_colorscales::THERMAL:
      args[COLOR_MAP]=kColorMapThermalPart;
      break;
    case dv::conf_colorscales::GRAYSCALE:
      args[COLOR_MAP]=kColorMapGrayscalePart;
      break;
  }
  switch (type) {
    case dv::conf_visualizationTypes::HEATMAP:
      args[MATRIX_TYPE]=kHeatMapTypePart;
      break;
    case dv::conf_visualizationTypes::SURFACE:
      args[MATRIX_TYPE]=kSurfaceTypePart;
      break;
    default:
      break;
  }
  args[TITLE] = dv::config().common.title;
  args[TITLE_X] = dv::config().common.xLabel;
  args[TITLE_Y] = dv::config().common.yLabel;
  make_string(kHtmlModel, args, page);
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
  string page;
  vector<string>args(MAX_ELEMENT,"");
  string str_values = "";
  createStringLineChartValues(values, str_values);
  args[VALUES] = str_values;
  args[TITLE] = title;
  args[TITLE_X] = dv::config().common.xLabel;
  args[TITLE_Y] = dv::config().common.yLabel;
  make_string(kHtmlModel, args, page);
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

//#STOP_GRAB_TO_DVS_NAMESPACE
}; // namespace dvs

