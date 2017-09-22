//
//  main.cpp
//  led_client
//
//  Created by Jack O'Shea on 8/30/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include <iostream>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "led_msg.h"
#include "led_msg_q.h"


int main(int argc, char * const argv[]) {
    
    led_msg_q q;
    try {
        q.init();
    } catch (...) {
        perror("Abort");
        exit(1);
    }
    
    char flag;
    char *options, *value, *kp[] = { NULL };
    while ((flag = getopt(argc, argv, "oOkKf:F:b:B:")) != -1 ) {
        led_msg msg;
        switch (flag) {
                
            // 'On' – no options
            case 'o':
            case 'O':
                msg = on_msg();
                break;
                
            // 'Kill' – no options
            case 'k':
            case 'K':
                msg = kill_msg();
                break;
                
            // 'Fill' – r,g,b,strip(TODO),start,end
            case 'F':
            case 'f':
                try {
                    options = optarg;
                    getsubopt(&options, kp, &value);
                    short r = atoi(suboptarg);
                    getsubopt(&options, kp, &value);
                    short g = atoi(suboptarg);
                    getsubopt(&options, kp, &value);
                    short b = atoi(suboptarg);
                    getsubopt(&options, kp, &value);
                    int start = atoi(suboptarg);
                    getsubopt(&options, kp, &value);
                    int end = atoi(suboptarg);
                    msg = fill_msg(r, g, b, 0, start, end);
                }
                catch (...){
                    fprintf(stderr, "Failed to parse fill command options");
                    continue;
                }
                break;
                
            // 'b' – brightness (out of 255)
            case 'b':
            case 'B':
                // TODO Do not use atoi!!! returns 0 on failure! ( I think )
                try {
                    msg = brightness_msg(atoi(optarg));
                } catch (...) {
                    fprintf(stderr, "Invalid brightness");
                    continue;
                }
                break;
                
            default:
                fprintf(stderr, "Invalid input");
                continue;
        }
        if (q.send(msg))
            printf("Sent %c\n", (char) msg.cmd);
        else
            perror("Msg error");
    }
    
    return 0;
}
