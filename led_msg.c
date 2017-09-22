//
//  led_msg.c
//  led_msg_q
//
//  Created by Jack O'Shea on 8/31/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include "led_msg.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 Constructors
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


led_msg brightness_msg(int brightness)
{
    return (led_msg) { 'b', { brightness } };
}

led_msg fill_msg(short r, short g, short b, char strip, int start, int end)
{
    return (led_msg) { 'f', { r, g, b, strip, start, end } };
}


led_msg on_msg()
{
    return (led_msg) { 'o' };
}
led_msg kill_msg()
{
    return (led_msg) { 'k' };
}



