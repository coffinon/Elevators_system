#include "elevator.h"

static uint8_t elevatorsCounter;

static void elevator_load(elevator_t *pEle, uint8_t index);
static void elevator_unload(elevator_t *pEle, uint8_t index);

/*
*   Function initializes new elevator
*/

elevator_t *create_new_elevator(void)
{
    elevator_t *ele = malloc(sizeof(elevator_t));
    
    ele->id =                       ++elevatorsCounter;
    ele->currentFloor =             1;
    ele->maxFloor =                 0;
    ele->minFloor =                 0;
    ele->state =                    STATE_IDLE;
    
    for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
    {
        ele->pas[i].id =            i + 1;
        ele->pas[i].entryFloor =    0;
        ele->pas[i].exitFloor =     0;
        ele->pas[i].state =         STATE_OUTSIDE;
    }
    
    return ele;
}

/*
*   Function prints given elevator status
*/

void print_elevator_status(elevator_t *pEle)
{
    printf("Elevator nr.%d status................................\n", pEle->id);
    printf("Current floor number :\t%d\n", pEle->currentFloor);
    printf("Max floor number :\t%d\n", pEle->maxFloor);
    printf("Min floor number :\t%d\n", pEle->minFloor);
    
    switch(pEle->state)
    {
        case STATE_IDLE :
            printf("Current state of the elevator :\tIDLE\n");
            break;
        case STATE_RUNNING_UP :
            printf("Current state of the elevator :\tRUNNING UP\n");
            break;
        case STATE_RUNNING_DOWN :
            printf("Current state of the elevator :\tRUNNING DOWN\n");
            break;
        case STATE_STOP :
            printf("Current state of the elevator :\tSTOP\n");
            break;
        default :
            printf("Invalid state of the elevator\n");
    }
    
    for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
    {
        printf("\nPassenger ID :\t%d\n", pEle->pas[i].id);
        printf("Passenger entry floor :\t%d\n", pEle->pas[i].entryFloor);
        printf("Passenger exit floor :\t%d\n", pEle->pas[i].exitFloor);
        
        switch(pEle->pas[i].state)
        {
            case STATE_INSIDE :
                printf("Passenger direction :\tINSIDE\n");
                break;
            case STATE_OUTSIDE :
                printf("Passenger direction :\tOUTSIDE\n");
        }
    }
    
    printf("\n\n");
}

/*
*   Function picks the passenger up
*/

void pickup_passenger(elevator_t *pEle, uint8_t entry_f, uint8_t exit_f)
{
    for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
    {
        if((pEle->pas[i].entryFloor == 0) && (pEle->pas[i].exitFloor == 0))
        {
            pEle->pas[i].entryFloor = entry_f;
            pEle->pas[i].exitFloor = exit_f;
            
            return;
        }
    }
    
    printf("Not enough space in the elevator\n\n\n");
}

/*
*   Function simulates elevator's behaviour
*/

void elevator_step(elevator_t *pEle)
{
    // Idle state handle
    
    if(pEle->state == STATE_IDLE)
    {
        // Reset maximum and minimum floor values
        
        pEle->maxFloor = pEle->currentFloor;
        pEle->minFloor = pEle->currentFloor;
        
        // Update maximum and minimum floor values
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            if(pEle->pas[i].entryFloor > pEle->maxFloor)
                pEle->maxFloor = pEle->pas[i].entryFloor;
            if((pEle->pas[i].entryFloor < pEle->minFloor) && pEle->pas[i].entryFloor)
                pEle->minFloor = pEle->pas[i].entryFloor;
        }
        
        // Decide in which direction you want to start moving
        
        if(pEle->maxFloor && (pEle->maxFloor > pEle->currentFloor))
            pEle->state = STATE_RUNNING_UP;
        else if(pEle->minFloor && (pEle->minFloor < pEle->currentFloor))
            pEle->state = STATE_RUNNING_DOWN;
        
        // Look if someone waits for an elevator on the current floor
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            if(pEle->pas[i].entryFloor == pEle->currentFloor)
                elevator_load(pEle, i);
            if(pEle->pas[i].entryFloor == pEle->currentFloor)
                elevator_unload(pEle, i);
        }
        
    }
    
    // Running up state handle
    
    else if(pEle->state == STATE_RUNNING_UP)
    {
        // Increment current floor value
        
        if(pEle->currentFloor < pEle->maxFloor)
            pEle->currentFloor++;
        
        // Look if someone waits for an elevator on the current floor
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            if((pEle->pas[i].entryFloor == pEle->currentFloor) && (pEle->pas[i].state == STATE_OUTSIDE))
                elevator_load(pEle, i);
            if((pEle->pas[i].exitFloor == pEle->currentFloor) && (pEle->pas[i].state == STATE_INSIDE))
                elevator_unload(pEle, i);
        }
        
        // Update maximum and minimum floor values
        
        uint8_t last_max_floor = pEle->maxFloor;
        
        pEle->maxFloor = 0;
        pEle->minFloor = pEle->currentFloor;
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            // Set new maximum floor value
            
            if((pEle->pas[i].entryFloor > pEle->maxFloor) && (pEle->pas[i].state == STATE_OUTSIDE))
                pEle->maxFloor = pEle->pas[i].entryFloor;
            if((pEle->pas[i].exitFloor > pEle->maxFloor) && (pEle->pas[i].state == STATE_INSIDE))
                pEle->maxFloor = pEle->pas[i].exitFloor;
            
            // Set new minimum floor value
            
            if((pEle->pas[i].entryFloor < pEle->minFloor) && (pEle->pas[i].state == STATE_OUTSIDE) && pEle->pas[i].entryFloor)
                pEle->minFloor = pEle->pas[i].entryFloor;
            if((pEle->pas[i].exitFloor < pEle->minFloor) && (pEle->pas[i].state == STATE_INSIDE) && pEle->pas[i].exitFloor)
                pEle->minFloor = pEle->pas[i].exitFloor;
        }
        
        // Change direction or go back to idle state
        
        last_max_floor = (pEle->maxFloor > last_max_floor) ? pEle->maxFloor : last_max_floor;
        
        if(pEle->currentFloor == last_max_floor)
        {
            if(pEle->minFloor < pEle->currentFloor)
            {
                pEle->maxFloor = pEle->currentFloor;
                pEle->state = STATE_RUNNING_DOWN;
            }
            else
            {
                pEle->maxFloor = pEle->currentFloor;
                pEle->maxFloor = pEle->currentFloor;
                pEle->state = STATE_IDLE;
            }
        }
    }
    
    // Running down state handle
    
    else if(pEle->state == STATE_RUNNING_DOWN)
    {
        // Increment current floor value
        
        if(pEle->currentFloor > pEle->minFloor)
            pEle->currentFloor--;
        
        // Look if someone waits for an elevator on the current floor
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            if((pEle->pas[i].entryFloor == pEle->currentFloor) && (pEle->pas[i].state == STATE_OUTSIDE))
                elevator_load(pEle, i);
            if((pEle->pas[i].exitFloor == pEle->currentFloor) && (pEle->pas[i].state == STATE_INSIDE))
                elevator_unload(pEle, i);
        }
        
        // Update maximum and minimum floor values
        
        uint8_t last_min_floor = pEle->minFloor;
        
        pEle->maxFloor = 0;
        pEle->minFloor = pEle->currentFloor;
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            // Set new maximum floor value
            
            if((pEle->pas[i].entryFloor > pEle->maxFloor) && (pEle->pas[i].state == STATE_OUTSIDE))
                pEle->maxFloor = pEle->pas[i].entryFloor;
            if((pEle->pas[i].exitFloor > pEle->maxFloor) && (pEle->pas[i].state == STATE_INSIDE))
                pEle->maxFloor = pEle->pas[i].exitFloor;
            
            // Set new minimum floor value
            
            if((pEle->pas[i].entryFloor < pEle->minFloor) && (pEle->pas[i].state == STATE_OUTSIDE) && pEle->pas[i].entryFloor)
                pEle->minFloor = pEle->pas[i].entryFloor;
            if((pEle->pas[i].exitFloor < pEle->minFloor) && (pEle->pas[i].state == STATE_INSIDE) && pEle->pas[i].exitFloor)
                pEle->minFloor = pEle->pas[i].exitFloor;
        }
        
        // Change direction or go back to idle state
        
        last_min_floor = (pEle->minFloor < last_min_floor) ? pEle->minFloor : last_min_floor;
        
        if(pEle->currentFloor == last_min_floor)
        {
            if(pEle->maxFloor > pEle->currentFloor)
            {
                pEle->minFloor = pEle->currentFloor;
                pEle->state = STATE_RUNNING_UP;
            }
            else
            {
                pEle->minFloor = pEle->currentFloor;
                pEle->maxFloor = pEle->currentFloor;
                pEle->state = STATE_IDLE;
            }
                
        }
    }
}

/*
*   Supporting function loading passenger to the elevator
*/

static void elevator_load(elevator_t *pEle, uint8_t index)
{
    pEle->pas[index].entryFloor = 0;
    pEle->pas[index].state = STATE_INSIDE;
}

/*
*   Supporting function loading passenger to the elevator
*/

static void elevator_unload(elevator_t *pEle, uint8_t index)
{
    pEle->pas[index].exitFloor = 0;
    pEle->pas[index].state = STATE_OUTSIDE;
}


