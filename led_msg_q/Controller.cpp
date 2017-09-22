//
//  Controller.cpp
//  led_msg_d
//
//  Created by Jack O'Shea on 9/7/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#include "Controller.hpp"

size_t Controller::totalStrips = 0;
std::vector<std::shared_ptr<Controller>> Controller::stripMap;
std::vector<std::shared_ptr<Controller>> Controller::all;


std::shared_ptr<Controller> Controller::create(std::string _portName, size_t stripCount) {
    try {
        std::shared_ptr<Controller> controller = std::shared_ptr<Controller>(new Controller(_portName, stripCount));
        stripMap.resize(totalStrips);
        all.push_back(controller);
        for (unsigned long i = totalStrips - stripCount; i < totalStrips; i++)
            stripMap.at(i) = controller;
        return controller;
    } catch (serial::IOException e) {
        fprintf(stderr, "Failure connecting to %s: %s", _portName.c_str(), e.what());
        return NULL;
    } catch (serial::SerialException e) {
        fprintf(stderr, "Failure connecting to %s: %s", _portName.c_str(), e.what());
        return NULL;
    } catch (serial::PortNotOpenedException e) {
        fprintf(stderr, "Failure connecting to %s: %s", _portName.c_str(), e.what());
        return NULL;
    }
}

Controller::Controller(std::string name, size_t _stripCount)
    : portName(name), stripCount(_stripCount), startStrip(totalStrips)
{

    totalStrips += _stripCount;
    device = std::make_shared<serial::Serial>(name, 4800, serial::Timeout::simpleTimeout(1000));
    if (device->isOpen()){
        printf("[Connected %lu] Device: %s\n", all.size(), device->getPort().c_str());
    } else {
        fprintf(stderr, "Could not open connection to %s", name.c_str());
    }
}

size_t Controller::getOnDeviceStripIndex(size_t stripIndex) {
    return stripIndex - stripMap.at(stripIndex)->startStrip;
}

size_t Controller::writeByStrip(size_t stripIndex, const uint8_t *data, size_t size) {
    return stripMap.at(stripIndex)->write(data, size);
}

size_t Controller::writeByController(size_t ardIndex, const uint8_t *data, size_t size) {
    return all.at(ardIndex)->write(data, size);
}

size_t Controller::write(const uint8_t* data, size_t size) {
    try {
        return device->write(data, size);
    } catch (...) {
        fprintf(stderr, "Write failure on %s", portName.c_str());
        return -1;
    }
    
}
