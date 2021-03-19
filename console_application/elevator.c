#include "elevator.h"

static uint8_t elevatorsCounter;

static void elevator_load(elevator_t *pEle, uint8_t index);
static void elevator_unload(elevator_t *pEle, uint8_t index);

/*
*   Function initializes new elevator
*/

elevator_t *elevator_create()
{
    // dynamically allocate data for the elevator structure and initialize its values
    elevator_t *pEle = malloc(sizeof(elevator_t));
    
    if(pEle != NULL)
    {
        pEle->id =                       ++elevatorsCounter;
        pEle->currentFloor =             1;
        pEle->maxFloor =                 1;
        pEle->minFloor =                 1;
        pEle->state =                    STATE_IDLE;
        
        for(int i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            pEle->pas[i].id =            i + 1;
            pEle->pas[i].entryFloor =    0;
            pEle->pas[i].exitFloor =     0;
            pEle->pas[i].state =         STATE_OUTSIDE;
        }
    }
    
    return pEle;
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
    // Check if entry and exit floors are between our highest and lowest floors
    
    if(((entry_f <= MAX_FLOORS_COUNT) && (entry_f > 0)) && ((exit_f <= MAX_FLOORS_COUNT) && (exit_f > 0)))
    {
        // Check if someone is not giving us data that makes no sense
        
        if(entry_f == exit_f)
        {
            printf("It makes no sense at all\n");
            return;
        }
        
        // Check if there is any free space in the queue
        
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
    else
        printf("Invalid entry / exit floor\n\n\n");
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

/*
*   Function gets information how many elevators the user want to simulate
*/

unsigned int get_elevator_count(void)
{
    unsigned int elevator_count;
    char num[10];
    bool is_number = true;
    
    while(1){
        // Clear the input buffer
        
        for(int i = 0; i < 10; ++i)
            num[i] = '\0';
        
        // Get user input
        
        printf("HOW MANY ELEVATORS DO YOU NEED ?\n");
        scanf("%s", num);
        
        clear_screen();
        
        // Check if input data includes digits only
        
        for(int i = 0; i < 10; ++i)
            if(!isdigit(num[i]) && num[i] != '\0')
            {
                is_number = false;
                break;
            }
        
        // Check if input data numbers are in specific range
        
        if(is_number)
        {
            elevator_count = atoi(num);
            
            if(elevator_count > MAX_ELEVATORS_COUNT)
                printf("MAXIMUM NUMBER OF ELEVATORS IS : %d\n", MAX_ELEVATORS_COUNT);
            else if(elevator_count < MIN_ELEVATORS_COUNT)
                printf("MINIMUM NUMBER OF ELEVATORS IS : %d\n", MIN_ELEVATORS_COUNT);
            else
                break;
        }
        else
        {
            printf("YOU CAN ONLY ENTER DIGITS AS AN INPUT\n");
            is_number = true;
        }
            
    }
    
    return elevator_count;
}

/*
*   Function clears the screen
*/

void clear_screen(void)
{
    printf("\e[1;1H\e[2J");
}
