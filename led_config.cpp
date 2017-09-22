//
//  led_config.cpp
//  led_msg_d
//
//  Created by Jack O'Shea on 9/22/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include "led_config.hpp"
#include <fstream>
#include <regex>
#include <vector>
#include "Controller.hpp"

std::string led_config::rc_path = "";

std::string led_config::get_path() {
    char  path [80] = {"\0"};
    strcat(path, getenv("HOME"));
    strcat(path, "/");
    const char * ledrc = ".ledrc";
    strcat(path, ledrc);
    rc_path = std::string(path);
    return rc_path;
}

std::string led_config::path() {
    return rc_path.length() ? rc_path : get_path();
}

void init_controller(std::string & name, std::vector<unsigned> sizes) {
    if (name.empty() || sizes.empty())
        return;
    if (Controller::create("/dev/" + name, sizes.size()) == NULL){
        throw name;
    }
    name = "";
    sizes.clear();
}

void led_config::init_controllers() {
    std::ifstream ledrc(path());
    std::string line;
    
    std::regex comment("\\s*#\\s*(.*)");
    std::regex devname("/dev/(\\S*)");
    std::regex numleds("\\s*(\\d+)");
    
    std::string device;
    std::vector<unsigned> sizes;
    
    while (getline(ledrc, line)) {
        std::smatch sm;
        if (std::regex_search(line, sm, comment)) continue;
        if (std::regex_search(line, sm, devname)) {
            init_controller(device, sizes);
            device = sm.str(1);
            printf("Device: %s\n", device.c_str());
            continue;
        }
        while (std::regex_search(line, sm, numleds)){
            printf("Strip Length: %s\n", sm.str(1).c_str());
            sizes.push_back((unsigned) strtoul(sm.str(1).c_str(), NULL, 10));
            line = sm.suffix();
        }
    }
    init_controller(device, sizes);
}
