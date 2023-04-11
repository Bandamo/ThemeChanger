#include "main.hpp"


const int test = 3;

int main(int argc, char** argv){
     ThemeChanger tc;
    
    tc.load_parameters();

    
    //while (1){

        tc.GET("data.json");

        // Load the json file
        ifstream data_file("data.json", ifstream::binary);

        json data = json::parse(data_file);

        cout << data["results"]["sunrise"] << endl;
        
        if (tc.changing_day()){ // If whe change the day

            tc.chg_dusk_to_dawn();
        }

        std::this_thread::sleep_for(seconds_t(1));
    //}
    return 0;
}