#include "CustomService.h"

CustomService::CustomService(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface)
{
}    