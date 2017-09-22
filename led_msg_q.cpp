//
//  led_msg_q.cpp
//  led_msg_d
//
//  Created by Jack O'Shea on 9/3/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include "led_msg_q.h"
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include "led_config.hpp"


// TODO try to open dotfile – probably json with led/arduino mapping
void led_msg_q::init() {

    std::string path = led_config::path();
    key_t qKey = ftok(path.c_str(), 'A');
    if (qKey == -1) {
        fprintf(stderr, "Error opening queue with file: %s\n", path.c_str());
        perror("ftok error");
        throw "ftok error";
    }

    q = msgget(qKey, 0666 | IPC_CREAT);
    if (q == -1) {
        perror("mssget error");
        throw "ftok error";
    }

}

// Send a message to the queue
bool led_msg_q::send(led_msg msg) {
    return msgsnd(q, &msg, sizeof(led_msg::content), 0) >= 0;
}

// True if success, false if error – BLOCKS
bool led_msg_q::receive(led_msg & msg){
    return msgrcv(q, &msg, sizeof(led_msg::content), 0, 0) >= 0;
}

