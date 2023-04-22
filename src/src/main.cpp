#include "main.hpp"


const int test = 3;

string pwd(){
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    string to_filter = string(buffer, (count > 0) ? count : 0);
    return to_filter.substr(0, to_filter.find_last_of("/"));
}

int main(int argc, char** argv){
    cout << "Nombre d'arguments : " << argc << endl;
    for(int i=0; i<argc; i++){
        cout << "Argument " << i << " : " << argv[i] << endl;
    }
    
    string data_path = pwd()+"/../lib/data.json";

    ThemeChanger tc;
    
    tc.load_parameters();

    if (tc.changing_day()){ // The day has changed
        tc.GET(data_path);
    }
    else{
        if (!tc.request_success()){ // The request isn't updated
            tc.GET(data_path);
        }
    }

    // Load the json file
    ifstream data_file(pwd()+"/../lib/data.json", ifstream::binary);

    try{
        json data = json::parse(data_file);
        cout << "\033[1;32mRequest successful\033[0m" << endl;
        tc.request_success_process();
        cout << "Request success processed" << endl;
        tc.get_sunset_and_sunrise(data);
        cout << "Sunrise and sunset got" << endl;
        cout << "Updating day or night..."<< endl;
        tc.update_day_or_night();
        cout << "Day or night updated" << endl;
    }
    catch(json::parse_error &e){
        cout << "\033[1;31mRequest error\033[0m" << endl;
        tc.request_error_process();
    }



    data_file.close();
    tc.save_parameters();

    return 0;

}