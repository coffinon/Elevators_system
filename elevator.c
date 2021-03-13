#include "elevator.h"


static void elevator_up(elevator_t *pEle);
static void elevator_down(elevator_t *pEle);

static uint8_t elevatorsCounter = 1;
static uint8_t passengersCounter = 0;

// --------------------------------------------------- structure initializations

/*
*   Function creates new elevator
*/

elevator_t *create_new_elevator(void)
{
    elevator_t *ele = malloc(sizeof(elevator_t));
    
    ele->id =                       elevatorsCounter++;
    ele->currentFloor =             1;
    ele->maxFloor =                 0;
    ele->minFloor =                 0;
    ele->nPassengers =              0;
    ele->state =                    STATE_IDLE;
    ele->bufferSize =               BUFFER_SIZE;
    ele->pBaseBufferAddr =          calloc(1, BUFFER_SIZE * sizeof(uint8_t));
    
    return ele;
}

/*
*   Function creates new passenger
*/

passenger_t *create_new_passenger(uint8_t entry_floor, uint8_t exit_floor)
{
    passenger_t *pas = malloc(sizeof(passenger_t));
    
    pas->id =                      passengersCounter++;
    pas->entryFloor =              entry_floor;
    pas->exitFloor =               exit_floor;
    
    return pas;
}

// ---------------------------------------------------------------- print status

/*
*   Function prints given elevator status
*/

void print_elevator_status(elevator_t *pEle)
{
    printf("Elevator nr.%d status...\n", pEle->id);
    printf("Current floor number :\t%d\n", pEle->currentFloor);
    printf("Max floor number :\t%d\n", pEle->maxFloor);
    printf("Min floor number :\t%d\n", pEle->minFloor);
    printf("Number of passengers :\t%d\n", pEle->nPassengers);
    
    switch(pEle->state)
    {
        case STATE_IDLE :
            printf("Current state of the elevator :\tIDLE\n");
            break;
        case STATE_MOVE_UP :
            printf("Current state of the elevator :\tRUNNING UP\n");
            break;
        case STATE_MOVE_DOWN :
            printf("Current state of the elevator :\tRUNNING DOWN\n");
    }
    
    printf("Maximum number of people in the elevator at the same time :\t%d\n", pEle->bufferSize);
    printf("Base address of the buffer :\t%d\n",  pEle->pBaseBufferAddr);
    
    printf("Values in the buffer :\t");
    for(int i = 0; i < pEle->bufferSize; ++i)
    {
        printf("%d ", pEle->pBaseBufferAddr[i]);
    }
    printf("\n\n");
}

// ---------------------------------------------------------- elevator scheduler

/*
*   Function adds passenger that is waiting for the elevetor to pick him up
*/

void elevator_step(elevator_t *pEle)
{
    if(pEle->state == STATE_IDLE)
    {
        pEle->maxFloor = 0;
        pEle->minFloor = pEle->currentFloor;
        
        for(int i = 0; i < MAX_NUMBER_OF_PASSENGERS; ++i)
        {
            if(pEle->pBaseBufferAddr[i] > pEle->maxFloor)
                pEle->maxFloor = pEle->pBaseBufferAddr[i];
            if((pEle->pBaseBufferAddr[i] < pEle->minFloor) && (pEle->pBaseBufferAddr[i] != 0))
                pEle->minFloor = pEle->pBaseBufferAddr[i];
        }
        if((pEle->maxFloor != 0) && (pEle->maxFloor > pEle->currentFloor))
        {
            pEle->state = STATE_MOVE_UP;
            pEle->minFloor = 0;
        }
        else if((pEle->minFloor != 0) && (pEle->minFloor < pEle->currentFloor))
        {
            pEle->state = STATE_MOVE_DOWN;
            pEle->maxFloor = 0;
        }
        else
            pEle->minFloor = 0;
    }
    else if(pEle->state == STATE_MOVE_UP)
    {
        if(pEle->currentFloor < pEle->maxFloor)
            pEle->currentFloor++;
        
        if(pEle->currentFloor == pEle->maxFloor)
        {
            for(int i = 0; i < MAX_NUMBER_OF_PASSENGERS * 2; ++i)
            {
                if(pEle->pBaseBufferAddr[i] > pEle->maxFloor)
                    pEle->maxFloor = pEle->pBaseBufferAddr[i];
            }
            if(pEle->currentFloor == pEle->maxFloor)
            {
                pEle->state = STATE_IDLE;
                pEle->maxFloor = 0;
            }
        }
        
        for(int i = 0; i < MAX_NUMBER_OF_PASSENGERS; ++i)
        {
            if(pEle->pBaseBufferAddr[i] == pEle->currentFloor)
            {
                pEle->pBaseBufferAddr[i] = 0;
                pEle->nPassengers++;
            }
        }
        
        for(int i = MAX_NUMBER_OF_PASSENGERS - 1; i < MAX_NUMBER_OF_PASSENGERS * 2; ++i)
        {
            if(pEle->pBaseBufferAddr[i] == pEle->currentFloor)
            {
                pEle->pBaseBufferAddr[i] = 0;
                pEle->nPassengers--;
            }
        }
    }
    else if(pEle->state == STATE_MOVE_DOWN)
    {
        if(pEle->currentFloor > pEle->minFloor)
            pEle->currentFloor--;
        
        if(pEle->currentFloor == pEle->minFloor)
        {
            for(int i = 0; i < MAX_NUMBER_OF_PASSENGERS * 2; ++i)
            {
                if((pEle->pBaseBufferAddr[i] < pEle->minFloor) && (pEle->pBaseBufferAddr[i] != 0))
                    pEle->minFloor = pEle->pBaseBufferAddr[i];
            }
            if(pEle->currentFloor == pEle->minFloor)
            {
                pEle->state = STATE_IDLE;
                pEle->minFloor = 0;
            }
        }
        
        for(int i = 0; i < MAX_NUMBER_OF_PASSENGERS; ++i)
        {
            if(pEle->pBaseBufferAddr[i] == pEle->currentFloor)
            {
                pEle->pBaseBufferAddr[i] = 0;
                pEle->nPassengers++;
            }
        }
        
        for(int i = MAX_NUMBER_OF_PASSENGERS - 1; i < MAX_NUMBER_OF_PASSENGERS * 2; ++i)
        {
            if(pEle->pBaseBufferAddr[i] == pEle->currentFloor)
            {
                pEle->pBaseBufferAddr[i] = 0;
                pEle->nPassengers--;
            }
        }
    }
}

// -------------------------------------------------------------- queue handling

/*
*   Function adds passenger that is waiting for the elevetor to pick him up
*/

void add_passenger(elevator_t *pEle, passenger_t *pPas)
{
    *(pEle->pBaseBufferAddr + pPas->id) = pPas->entryFloor;
    *(pEle->pBaseBufferAddr + pPas->id + MAX_NUMBER_OF_PASSENGERS) = pPas->exitFloor;
    free(pPas);
}

