//
//  main.cpp
//  led_msg_q
//
//  Created by Jack O'Shea on 8/30/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "led_msg.h"
#include "led_msg_q.h"
#include "led_config.hpp"

#include "serial.h"

#include "Controller.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                     Devices
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void
printDeviceList() {
    std::vector<serial::PortInfo> ports = serial::list_ports();
    printf("Available Devices:\n");
    
    for (serial::PortInfo port : ports) {
        printf("%s: %s\n", port.port.c_str(), port.description.c_str());
    }
}

// To send a fill msg to device:
// Change the global strip index to the local strip index
// Send to the corresponding device
void
sendFill(struct led_msg::content::fill fill) {
    printf("fill #%x%x%x from %d to %d on strip %d\n",
           fill.r,     fill.g,   fill.b,
           fill.start, fill.end, fill.strip);
    uint8_t onDeviceIndex = Controller::getOnDeviceStripIndex(fill.strip);
    Controller::writeByStrip(fill.strip, (uint8_t*) "f", sizeof(char));
    Controller::writeByStrip(fill.strip, (uint8_t*) &fill.r, sizeof(fill.r));
    Controller::writeByStrip(fill.strip, (uint8_t*) &fill.g, sizeof(fill.g));
    Controller::writeByStrip(fill.strip, (uint8_t*) &fill.b, sizeof(fill.b));
    Controller::writeByStrip(fill.strip, (uint8_t*) &onDeviceIndex, sizeof(onDeviceIndex));
    Controller::writeByStrip(fill.strip, (uint8_t*) &fill.start, sizeof(fill.start));
    Controller::writeByStrip(fill.strip, (uint8_t*) &fill.end, sizeof(fill.end));
}


void
connectFailed(std::string device) {
    fprintf(stderr, "\nCould not open device specified in config.\n\n");
    printDeviceList();
    fprintf(stderr, "\nAborting...\n");
    exit(2);
}

void
connectToDevices(std::vector<std::string>::iterator portname,
                 std::vector<std::string>::iterator stop) {
    if (portname == stop)
        return;
    try {
        if (Controller::create(*portname, 1) == NULL)
            connectFailed(*portname);
    } catch (...) {
        connectFailed(*portname);
    }
    connectToDevices(++portname, stop);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                  Process Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void
route_msg(led_msg msg) {
    
    union led_msg::content content = msg.content;
    
    switch (msg.cmd) {
        case 'f':
            sendFill(msg.content.fill);
            break;
            
        case 'b':
            printf("setting %d for global brightness\n", content.brightness);
            break;
            
        default:
            printf("unknown command %c\n", (char) msg.cmd);
    }
}


int
main(int argc, const char * argv[]) {
    

    try {
        led_config::init_controllers();
    } catch (...) {
        connectFailed("");
    }
    
    led_msg_q q;
    try {
        q.init();
    } catch (const char * error) {
        fprintf(stderr, "Could not open msg_q: %s\n", error);
        exit(1);
    } catch (...) {
        perror("Could not open msg_q");
    }

    printf("Daemon running\n");
    
    led_msg msg;
    while (q.receive(msg)) {
        route_msg(msg);
    }
    perror("msgrcv error");
    
    return 1;
}
