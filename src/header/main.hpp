#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

#include <chrono>
#include <thread>
#include <string>
#include <boost/filesystem.hpp>

using namespace std;
using seconds_t = std::chrono::seconds;
using json = nlohmann::json;

string pwd();

class ThemeChanger{
    public:
        ThemeChanger();
        int get_day_since_epoch();
        void chg_dusk_to_dawn();
        bool changing_day();
        void update_day();
        void load_parameters();
        std::string GET(std::string file);
        int32_t convert_time_to_epoch(std::string t);
        void update_day_or_night(json data);
    private:
        void ChangeTheme(bool don);
        uint16_t day;
        uint8_t period;
        uint16_t current_day;
        string url;
        string lat;
        string lon;
        int32_t current_epoch;
        int32_t sunrise_epoch;
        int32_t sunset_epoch;
        bool day_or_night; // True if day, false if night
};