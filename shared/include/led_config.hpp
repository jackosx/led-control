//
//  led_config.hpp
//  led_msg_d
//
//  Created by Jack O'Shea on 9/22/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#ifndef led_config_hpp
#define led_config_hpp

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

class led_config {
    static std::string rc_path;
    static std::string get_path();
    
public:
    static std::string path();
    static void init_controllers();
};


#endif /* led_config_hpp */
