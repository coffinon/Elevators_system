#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE                 10U

#define STATE_IDLE                  0U
#define STATE_MOVE_UP               1U
#define STATE_MOVE_DOWN             2U

#define MAX_NUMBER_OF_PASSENGERS    5U
#define MAX_NUMBER_OF_FLOORS        10U
#define MAX_NUMBER_OF_ELEVATORS     16U

typedef struct
{
    uint8_t id;
    uint8_t currentFloor;
    uint8_t maxFloor;
    uint8_t minFloor;
    uint8_t nPassengers;
    uint8_t state;
    uint8_t bufferSize;
    uint8_t *pBaseBufferAddr;
} elevator_t;

typedef struct
{
    uint8_t id;
    uint8_t entryFloor;
    uint8_t exitFloor;
} passenger_t;


elevator_t *create_new_elevator(void);

passenger_t *create_new_passenger(uint8_t entry_floor, uint8_t exit_floor);

void print_elevator_status(elevator_t *pEle);

void elevator_step(elevator_t *pEle);

void add_passenger(elevator_t *pEle, passenger_t *pPas);


#endif /* _ELEVATOR_H_ */