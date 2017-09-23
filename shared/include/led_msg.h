//
//  led_msg.h
//  led_msg_q
//
//  Created by Jack O'Shea on 8/31/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#ifndef led_msg_h
#define led_msg_h

#include <stdint.h>

typedef struct led_msg {
    long cmd; // char ie 'f' or 'b'
    union content {
        struct fill {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t strip;
            uint8_t start;
            uint8_t end;
        } fill;
        short brightness;
    } content;
} led_msg;


#ifdef __cplusplus
extern "C" {
#endif
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 Constructors
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

led_msg brightness_msg(int brightness);
led_msg fill_msg(short r, short g, short b, char strip, int start, int end);
led_msg on_msg();
led_msg kill_msg();

    
#ifdef __cplusplus
}
#endif
    
#endif /* led_msg_h */
