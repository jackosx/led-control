//
//  led_msg_q.h
//  led_msg_d
//
//  Created by Jack O'Shea on 9/3/17.
//  Copyright © 2017 Jack O'Shea. All rights reserved.
//

#ifndef led_msg_q_h
#define led_msg_q_h

#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "led_msg.h"


// Access the IPC Message queue for led control
class led_msg_q {

    // pretty typing
    typedef int qid_t;
    qid_t q;
    
public:
    // Attempt to setup the queue. Throws
    void init();
    
    // Send a message to the queue
    bool send(led_msg msg);
    
    // True if success, false if error – BLOCKS
    bool receive(led_msg & msg); // TODO Implement this
        
};

#endif /* led_msg_q_h */
