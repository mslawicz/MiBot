 
#ifndef CUSTOM_SERVICE_H_
#define CUSTOM_SERVICE_H_
 
#include <mbed.h>
#include "ble/BLE.h"

class CustomService
{
public:
    CustomService(events::EventQueue& eventQueue, BLE& bleInterface);
private:
    events::EventQueue& eventQueue;     // event queue of the main thread
    BLE& bleInterface;                  // interface to BLE device
};

#endif /* CUSTOM_SERVICE_H_ */