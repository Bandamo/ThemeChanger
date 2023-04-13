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

    url = "\"https://api.sunrise-sunset.org/json?lat=" + lat + "&lng=" + lon +"&formatted=0&date=today\"";
    cout << "url set to :" << url << endl;
}

int32_t ThemeChanger::convert_time_to_epoch(string t){
    struct tm *timeptr,result;
    char buf[1024];
    strncpy(buf, t.c_str(), sizeof(buf));
    buf[sizeof(buf) - 1] = 0;
    strptime(buf, "%Y-%m-%dT%H:%M:%S+00:00",&result);
    int32_t epoch_time = result.tm_sec + result.tm_min*60 + result.tm_hour*3600 + result.tm_yday*86400 + (result.tm_year-70)*31536000 + ((result.tm_year-69)/4)*86400 - ((result.tm_year-1)/100)*86400 + ((result.tm_year+299)/400)*86400;
    return epoch_time;
}

void ThemeChanger::update_day_or_night(json data){
    string sunrise = data["results"]["sunrise"];
    string sunset = data["results"]["sunset"];

    sunrise_epoch = convert_time_to_epoch(sunrise);
    sunset_epoch = convert_time_to_epoch(sunset);

    current_epoch = time(nullptr);

    bool prec_day_or_night = day_or_night;

    if (current_epoch > sunrise_epoch && current_epoch < sunset_epoch){
        day_or_night = true;
    }
    else{
        day_or_night = false;
    }

    if (prec_day_or_night != day_or_night){
        ChangeTheme(day_or_night);
    }
}

void ThemeChanger::ChangeTheme(bool don){
    string cmd;

    if (don){
        cmd = "."+pwd()+"/../src/src/from_night_to_day.sh";
    }
    else{
        cmd = "."+pwd()+"/../src/src/from_day_to_night.sh";
    }

    system(cmd.c_str());
}