//
//  Controller.hpp
//  led_msg_d
//
//  Created by Jack O'Shea on 9/7/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//
//  Why so much static? I only want one connection to each device.
//  There isn't even a good reason for a client to have access
//  to an individual instance, just need the most simple way to send a
//  message based on an led strip number.
//
//  Why not use a vector then if its going to be index-based messaging?
//  We need a more complex data structure to map LED strips to controllers
//  Maybe could've just used a map, but at the very least this is an
//  exercise. I'm also not sure that would store enough info because before
//  sending the fill message the "strip" field needs to be replaced with
//  the strip's on device index, so simple knowing the LEDStrip to controller
//  mapping isn't enough.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <stdio.h>
#include <stdlib.h>
#include "serial.h"

class Controller {
private:
    Controller()  = delete;
    Controller(std::string _portName, size_t _stripCount);
    static size_t              totalStrips;
    static std::vector<size_t> strips;          // Number of LEDs on any particular strip TODO is this needed?
    static std::vector<std::shared_ptr<Controller>> stripMap;  // Strip => Controlling Arduino
    static std::vector<std::shared_ptr<Controller>> all;  // Strip => Controlling Arduino
    
    std::shared_ptr<serial::Serial> device;
    
    const std::string portName;
    const size_t startStrip;   // Gobal(?) index of the first strip controlled
    const size_t stripCount;   // Number of strips this controller has
    
public:
    // Only way for a client to create a controller.
    // Returns a pointer to the newly created instance, but the client doesn't need
    // to hold this. Also creates a static reference to the new controller that
    // will persist for the program's lifetime.
    static std::shared_ptr<Controller> create(std::string name, size_t _stripCount);
    
    // Send data to the controller responsible for the ledIndex'th strip
    static size_t writeByStrip(size_t stripIndex, const uint8_t* data, size_t size);
    
    // Send data to a specific controller (arduino)
    static size_t writeByController(size_t ardIndex, const uint8_t* data, size_t size);
    
    // Simple write method.
    size_t write(const uint8_t* data, size_t size);
    
    // Map a strip's global index to the index it has on its controlling device
    // This is needed for sending commands to a controller that target a specific
    // strip
    static size_t getOnDeviceStripIndex(size_t stripIndex);
    
};

#endif /* Controller_hpp */
