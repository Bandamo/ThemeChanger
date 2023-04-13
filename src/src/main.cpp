#include "main.hpp"


const int test = 3;

string pwd(){
    char buffer[1024];
    ssize_t count = readlink("/proc/self/exe", buffer, 1024);
    string to_filter = string(buffer, (count > 0) ? count : 0);
    return to_filter.substr(0, to_filter.find_last_of("/"));
}

int main(int argc, char** argv){
     ThemeChanger tc;
    
    tc.load_parameters();

    
    while (1){
        tc.GET("data.json");

        cout << pwd() << endl;

        // Load the json file
        ifstream data_file(pwd()+"/data.json", ifstream::binary);

        json data = json::parse(data_file);

        string date = data["results"]["sunrise"];

        data_file.close();

        std::this_thread::sleep_for(seconds_t(0.1));
    }
    return 0;
}