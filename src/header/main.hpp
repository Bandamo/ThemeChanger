#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

#include <chrono>
#include <thread>
#include <string>

using namespace std;
using seconds_t = std::chrono::seconds;
using json = nlohmann::json;

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
    private:
        uint16_t day;
        uint8_t period;
        uint16_t current_day;
        string url;
        string lat;
        string lon;

};