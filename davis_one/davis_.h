#ifndef DAVIS_H_
#define DAVIS_H_

#include <iostream>
#include <memory>
#include <vector>
#include <string>
namespace {

}// namespace end
namespace davis {
extern const char kAppName[];
extern const char kOutFolderName[];
extern const char kPlotlyJsName[];
extern const char kPlotlyJsResourcePath[];
extern const char kPlotlyJsWorkPath[];
extern const char kCommonHeadPart[];
extern const char kDivSizePart[];
extern const char kColorMapDefaultPart[];
extern const char kColorMapSunnyPart[];
extern const char kColorMapGlamourPart[];
extern const char kColorMapThermalPart[];
extern const char kHeatMapTypePart[];
extern const char kSurfaceTypePart[];
extern const char kCommonLastPart[];
using std::string;
using std::vector;

string getCurrentPath();

bool isPlotlyScriptExists();

bool saveStringToFile(const string& file_name,
                      const string& data);

void mayBeCreateJsWorkingFolder();

void sleepMs(unsigned long milisec);

void openPlotlyHtml(const string& file_name);

bool getDataFromFile(const string& path, string& result);

vector<string> split(const string& target, char c);

bool readMatrix(vector<vector<double>>& outMatrix, const string& path, char dlmtr);


// Now it doesn't work.
bool deleteFolder(const char* fname);


using std::vector;

enum class visualizationTypes {
  CHART,
  HEATMAP,
  SURFACE
};

enum class colorscales {
  DEFAULT,
  SUNNY,
  GLAMOUR,
  THERMAL
};

class ShowSettings {
 public:
  virtual ~ShowSettings() {}
  visualizationTypes getVisualType() const;

 protected:
  visualizationTypes visualType;
};

class ShowSettingsHeatMap : public ShowSettings {
 public:
  ShowSettingsHeatMap(colorscales color = colorscales::DEFAULT) {
    visualType = visualizationTypes::HEATMAP;
    colorScale = color;
  }
  colorscales colorScale;
};

class ShowSettingsSurface : public ShowSettings {
 public:
  ShowSettingsSurface(colorscales color = colorscales::DEFAULT) {
    visualType = visualizationTypes::SURFACE;
    colorScale = color;
  }
  colorscales colorScale;
};

class ShowSettingsChart : public ShowSettings {
 public:
  ShowSettingsChart() {
    visualType = visualizationTypes::CHART;
  }
};

std::unique_ptr<ShowSettingsHeatMap> createShowSettingsHeatMap(colorscales color = colorscales::DEFAULT);
std::unique_ptr<ShowSettingsSurface> createShowSettingsSurface(colorscales color = colorscales::DEFAULT);
std::unique_ptr<ShowSettingsChart> createShowSettingsChart();

bool createHtmlPageWithPlotlyJS(const vector<vector<double>>& values,
                                std::string& page,
                                const visualizationTypes& visualType,
                                const colorscales& colorscale);

bool showHeatMapInBrowser(const vector<vector<double>>& values, const std::string& title,
                          const ShowSettingsHeatMap* settings);

bool showHeatMapInBrowser(const std::string& values, const std::string& title,
                          const ShowSettingsHeatMap* settings);

bool showLineChartInBrowser(const vector<double>& values, const std::string& title,
                            const ShowSettingsChart* settings);

bool showLineChartInBrowser(const std::string& values, const std::string& title,
                            const ShowSettingsChart* settings);

bool showSurfaceInBrowser(const vector<vector<double>>& values, const std::string& title,
                          const ShowSettingsSurface* settings);

bool showSurfaceInBrowser(const std::string& values, const std::string& title,
                          const ShowSettingsSurface* settings);

using std::vector;
using std::string;


//! two-dimensional vector
template <typename T>
bool show(const vector<vector<T>>& data, const string& title = kAppName,
          ShowSettings* settings = nullptr);

//! two-dimensional array
template <typename T>
bool show(T** data, uint64_t arrRows, uint64_t arrCols,
          const string& title = kAppName,  ShowSettings* settings = nullptr);

//! a one-dimensional array that simulates a two-dimensional one (element access [i*cols+j])
template <typename T>
bool show(const T* data, uint64_t arrRows, uint64_t arrCols,
          const string& title = kAppName,  ShowSettings* settings = nullptr);

//! one-dimensional vector
template <typename T>
bool show(const vector<T>& data, const string& title = kAppName,
          ShowSettings* settings = nullptr);

//! one-dimensional array
template <typename T>
bool show(const T* data, uint64_t count, const string& title = kAppName,
          ShowSettings* settings = nullptr);

// ***********************************
// template functions implementations:
// ***********************************

template <typename T>
bool show(const vector<vector<T>>& data, const string& title,
          ShowSettings* settings) {
  vector<vector<double>> vecVecDbl;
  vecVecDbl.reserve(data.size());
  for (vector<T> row : data) {
    vector<double> dblRow(row.begin(), row.end());
    vecVecDbl.emplace_back(dblRow);
  }
  bool res = false;
  std::unique_ptr<ShowSettings> smartP; //Smart pointer for auto deleting settings if it will be created bellow via NEW
  if (settings == nullptr) {
    settings = new ShowSettingsHeatMap();
    smartP.reset(settings);
  }
  if (settings->getVisualType() == davis::visualizationTypes::HEATMAP)
    res = davis::showHeatMapInBrowser(vecVecDbl, title, static_cast<const ShowSettingsHeatMap*>(settings));
  else if (settings->getVisualType() == davis::visualizationTypes::SURFACE)
    res = davis::showSurfaceInBrowser(vecVecDbl, title, static_cast<const ShowSettingsSurface*>(settings));
  return res;
}

template <typename T>
bool show(T** data, uint64_t arrRows, uint64_t arrCols, const string& title,
          ShowSettings* settings) {
  vector<vector<double>> vecVecDbl;
  vecVecDbl.reserve(arrRows);
  for (uint64_t i = 0; i < arrRows; ++i) {
    vector<double> dblRow(&data[i][0], &data[i][0] + arrCols);
    vecVecDbl.emplace_back(dblRow);
  }
  bool res = false;
  std::unique_ptr<ShowSettings> smartP; //Smart pointer for auto deleting settings if it will be created bellow via NEW
  if (settings == nullptr) {
    settings = new ShowSettingsHeatMap();
    smartP.reset(settings);
  }
  if (settings->getVisualType() == davis::visualizationTypes::HEATMAP)
    res = davis::showHeatMapInBrowser(vecVecDbl, title, static_cast<const ShowSettingsHeatMap*>(settings));
  else if (settings->getVisualType() == davis::visualizationTypes::SURFACE)
    res = davis::showSurfaceInBrowser(vecVecDbl, title, static_cast<const ShowSettingsSurface*>(settings));
  return res;
}

template <typename T>
bool show(const T* data, uint64_t arrRows, uint64_t arrCols, const string& title,
          ShowSettings* settings) {
  vector<vector<double>> vecVecDbl;
  vecVecDbl.reserve(arrRows);
  for (uint64_t i = 0; i < arrRows; ++i) {
    vector<double> dblRow(&data[i * arrCols], &data[i * arrCols] + arrCols);
    vecVecDbl.emplace_back(dblRow);
  }
  bool res = false;
  std::unique_ptr<ShowSettings> smartP; //Smart pointer for auto deleting settings if it will be created bellow via NEW
  if (settings == nullptr) {
    settings = new ShowSettingsHeatMap();
    smartP.reset(settings);
  }
  if (settings->getVisualType() == davis::visualizationTypes::HEATMAP)
    res = davis::showHeatMapInBrowser(vecVecDbl, title, static_cast<const ShowSettingsHeatMap*>(settings));
  else if (settings->getVisualType() == davis::visualizationTypes::SURFACE)
    res = davis::showSurfaceInBrowser(vecVecDbl, title, static_cast<const ShowSettingsSurface*>(settings));
  return res;
}

template <typename T>
bool show(const vector<T>& data, const string& title,
          ShowSettings* settings) {
  vector<double> dblRow(data.begin(), data.end());
  bool res = false;
  std::unique_ptr<ShowSettings> smartP; //Smart pointer for auto deleting settings if it will be created bellow via NEW
  if (settings == nullptr) {
    settings = new ShowSettingsChart();
    smartP.reset(settings);
  }
  if (settings->getVisualType() == davis::visualizationTypes::CHART)
    res = davis::showLineChartInBrowser(dblRow, title, static_cast<const ShowSettingsChart*>(settings));
  return res;
}

template <typename T>
bool show(const T* data, uint64_t count, const string& title,
          ShowSettings* settings) {
  vector<double> dblRow(data, data + count);
  bool res = false;
  std::unique_ptr<ShowSettings> smartP; //Smart pointer for auto deleting settings if it will be created bellow via NEW
  if (settings == nullptr) {
    settings = new ShowSettingsChart();
    smartP.reset(settings);
  }
  if (settings->getVisualType() == davis::visualizationTypes::CHART)
    res = davis::showLineChartInBrowser(dblRow, title, static_cast<const ShowSettingsChart*>(settings));
  return res;
}

} // namespace davis end
#endif // DAVIS_H_