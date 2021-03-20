#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PASSENGERS_COUNT    10

#define MIN_FLOORS_COUNT        2
#define MAX_FLOORS_COUNT        10

#define MIN_ELEVATORS_COUNT     2
#define MAX_ELEVATORS_COUNT     16

#define STATE_IDLE              0
#define STATE_RUNNING_UP        1
#define STATE_RUNNING_DOWN      2

#define STATE_INSIDE            0
#define STATE_OUTSIDE           1


typedef struct
{
    uint8_t id;
    uint8_t entryFloor;
    uint8_t exitFloor;
    uint8_t state;
} passenger_t;


typedef struct
{
    uint8_t id;
    uint8_t currentFloor;
    uint8_t maxFloor;
    uint8_t minFloor;
    uint8_t state;
    passenger_t pas[MAX_PASSENGERS_COUNT];
} elevator_t;


elevator_t *elevator_create(void);

void print_elevator_status(elevator_t *pEle);

bool pickup_passenger(elevator_t *pEle, uint8_t entry_f, uint8_t exit_f);

void elevator_step(elevator_t *pEle);

unsigned int get_elevator_count(void);

void clear_screen(void);

#endif /* _ELEVATOR_H_ */