#include "main.hpp"

ThemeChanger::ThemeChanger(){
    day = get_day_since_epoch();
    current_day = day;
    updated_request = false;
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

bool ThemeChanger::changing_day(){
    bool result;
    result = (day != current_day);
    if (result){ 
        // The day has changed, request is no longer up-to-date
        updated_request = false;
    }
    return result;
}

void ThemeChanger::update_day(){
    day=current_day;
}

bool ThemeChanger::request_success(){
    return updated_request;
}

void ThemeChanger::request_error_process(){
    updated_request = false; 
    sunrise_epoch = default_sunrise;
    sunset_epoch = default_sunset;
}

void ThemeChanger::request_success_process(){
    updated_request = true;
}

void ThemeChanger::load_parameters(){
    // Load the json file
    ifstream data_file(pwd()+"/../lib/param.json", ifstream::binary);

    json data = json::parse(data_file);
    
    lon = data["lon"];
    lat = data["lat"];
    day = data["day"];
    default_sunrise = data["default_sunrise"];
    default_sunset = data["default_sunset"];
    updated_request = data["updated_request"];

    url = "\"https://api.sunrise-sunset.org/json?lat=" + lat + "&lng=" + lon +"&formatted=0&date=today\"";
    cout << "url set to :" << url << endl;
}

void ThemeChanger::save_parameters(){
    json param;
    param["lon"] = lon;
    param["lat"] = lat;
    param["day"] = day;
    param["default_sunrise"] = day * 86400 + 6*3600; // Default sunrise at 6:00
    param["default_sunset"] = day * 86400 + 18*3600; // Default sunset at 18:00
    param["updated_request"] = updated_request;
    ofstream param_file(pwd()+"/../lib/param.json", ofstream::binary);
    param_file << param;
    param_file.close();
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

void ThemeChanger::get_sunset_and_sunrise(json data){
    string sunrise = data["results"]["sunrise"];
    string sunset = data["results"]["sunset"];

    sunrise_epoch = convert_time_to_epoch(sunrise);
    sunset_epoch = convert_time_to_epoch(sunset);
}

void ThemeChanger::update_day_or_night(){
    current_epoch = time(nullptr);

    bool prec_day_or_night = day_or_night;

    if (current_epoch > sunrise_epoch && current_epoch < sunset_epoch){
        day_or_night = true;
    }
    else{
        day_or_night = false;
    }

    if (prec_day_or_night != day_or_night){
        ChangeTheme();
    }
}

void ThemeChanger::ChangeTheme(){
    string cmd;

    cout << "Day or night : " << day_or_night << endl;

    if (day_or_night){
        cmd = pwd()+"/../src/src/from_dark_to_light.sh";
    }
    else{
        cmd = pwd()+"/../src/src/from_light_to_dark.sh";
    }

    system(cmd.c_str());
}