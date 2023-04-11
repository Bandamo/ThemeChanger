#include "main.hpp"

ThemeChanger::ThemeChanger(){
    day = get_day_since_epoch();
    current_day = day;
    period = 0; // 0 for de dawn, 1 for the day, 2 for the dusk
    cout << "ThemeChanger created" << endl;
}

int ThemeChanger::get_day_since_epoch(){
    const auto now = std::chrono::system_clock::now();
    const auto now_sec = std::chrono::time_point_cast<seconds_t>(now);
    const auto value = now_sec.time_since_epoch();
    const auto duration = value.count();
    return duration/86400;
}

string ThemeChanger::GET(string file) {
    /*
        Write the result of the curl command on url in the file file
    */
    std::string cmd = "curl -s " + url;
    FILE *p = popen(cmd.c_str(), "r");
    char buffer[2];
    std::string body;
    if (!p) {
        return "Error";
    }
    while (fgets(buffer, sizeof(buffer), p)) {
        body += buffer;
    }
    ofstream f;
    f.open(file);
    f << body;
    f.close();
    return body;
}

void ThemeChanger::chg_dusk_to_dawn(){
    if (period == 2){
        period = 0;
    }
}

bool ThemeChanger::changing_day(){
    bool result;
    result = (day != current_day);
    return result;
}

void ThemeChanger::update_day(){
    day=current_day;
}

void ThemeChanger::load_parameters(){
    // Load the json file
    ifstream data_file("../lib/param.json", ifstream::binary);

    json data = json::parse(data_file);

    lon = data["lon"];
    lat = data["lat"];

    url = "https://api.sunrise-sunset.org/json?lat=" + lat + "&lng=" + lon +"&date=today";
}
