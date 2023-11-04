#include "common_utils.h"
#include "fstream"

namespace{
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#elif
#include <unistd.h>
#endif

inline bool is_file_exists (const std::string &file_name) {
    std::ifstream file(file_name.c_str());
    if(!file)return false;
    return true;
}


#if defined(WIN32) || defined(NT)
std::string command = "start ";
#elif APPLE
std::string command = "open ";
#elif linux
std::string command = "xdg-open";
#else
#error "Unknown compiler"
#endif

void openFileBySystem(const std::string &file_name){
    command.append(davis::getCurrentPath());
    command.append("\\");
    command.append(file_name);
    system(command.c_str());
}

} // namespace

namespace davis {

std::string getCurrentPath(){
    char buffer[1024];
    char *answer = getcwd(buffer, sizeof(buffer));
    std::string s_cwd;
    if (answer)
    {
        s_cwd = answer;
    }
    return s_cwd;
};

bool isPlotlyScriptExists(){
    return is_file_exists("plotly-2.27.0.min.js");
};

bool saveStringToFile(const std::string file_name,
                      const std::string &data){

    std::ofstream out(file_name);
    if(out.is_open()){
        out << data.c_str();
        return false;
        out.close();
    }
    return true;
}


void openPlotlyHtml(){
    openFileBySystem("example.html");
}

}; // namespace davis
