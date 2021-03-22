#include "elevator_unit_test.h"

#ifdef _WIN32
#include <inttypes.h>
#define PASSED "PASSED"
#define FAILED "FAILED"

#ifdef _WIN64
#define PRI_SIZET PRIu64
#else
#define PRI_SIZET PRIu32
#endif
#else
#define PRI_SIZET "zu"
#define PASSED "\x1b[32mPASSED\x1b[0m"
#define FAILED "\x1b[31mFAILED\x1b[0m"

#endif

#define test(expr)(!(expr)) ? (printf(FAILED" at line %u\n", __LINE__), false) : (printf(PASSED"\n"), true)
#define test_quiet(expr)(!(expr)) ? (printf(FAILED" at line %u\n", __LINE__), false) : true

static unsigned int elevator_current_id;

/*
*   Elevator initialization unit test
*/

static elevator_t *elevator_init_ut(void)
{
    printf("Test 1: Checking elevator_init...\n");
    bool result = true;
    elevator_t *pEle = elevator_create();
    
    if(pEle == NULL)
        result = false;
    
    if(true == result) {
        printf("     Has unique ID: ");
        result &= test(pEle->id == ++elevator_current_id);
        printf("     Current floor is 1: ");
        result &= test(pEle->currentFloor == 1);
        printf("     Max floor is 1: ");
        result &= test(pEle->maxFloor == 1);
        printf("     Min floor is 1: ");
        result &= test(pEle->minFloor == 1);
        printf("     State is IDLE: ");
        result &= test(pEle->state == STATE_IDLE);
        
        for(uint8_t i = 0; i < MAX_PASSENGERS_COUNT; ++i)
        {
            printf("     #%d Passenger has unique ID: ", i + 1);
            result &= test(pEle->pas[i].id == i + 1);
            printf("     #%d Passenger entry floor: ", i + 1);
            result &= test(pEle->pas[i].entryFloor == 0);
            printf("     #%d Passenger exit floor: ", i + 1);
            result &= test(pEle->pas[i].exitFloor == 0);
            printf("     #%d Passenger state: ", i + 1);
            result &= test(pEle->pas[i].state == STATE_OUTSIDE);
        }
    }
    
    printf("Test 1: %s\n\n", (true == result) ? PASSED : FAILED);
    return pEle;
}

/*
*   Elevator pickup function unit test
*/

static bool elevator_pickup_ut(elevator_t *pEle)
{
    printf("Test 2: Pickup test...\n");
    bool result = true;
    
    uint8_t entry_floor = 1;
    uint8_t exit_floor = 1;
    
    for(uint8_t i = 0; entry_floor + i <= MAX_FLOORS_COUNT + 1; ++i)
    {   
        if(entry_floor == exit_floor + i || i >= MAX_FLOORS_COUNT)
        {
            printf("     #%d Passenger request not picked up : ", i + 1);
            result &= !test(pickup_passenger(pEle, entry_floor, exit_floor + i));
        }
        else
        {
            printf("     #%d Passenger request picked up : ", i + 1);
            result &= test(pickup_passenger(pEle, entry_floor, exit_floor + i));
        }
    }
    printf("Test 2 : %s\n\n", (true == result) ? PASSED : FAILED);
    
    return result;
}

/*
*   Elevator running up unit test
*/

static bool elevator_running_up_ut(elevator_t *pEle)
{
    printf("Test 3: Running up test...\n");
    bool result = true;
    
    for(uint8_t i = 0; i < MAX_FLOORS_COUNT; ++i)
    {
        elevator_step(pEle);
        if(i > 0)
        {
            printf("     #%d Passenger dropped out : ", i);
            result &= test(pEle->pas[i - 1].state == STATE_OUTSIDE);
        }
    }
    printf("Test 3: %s\n\n", (true == result) ? PASSED : FAILED);
    
    return result;
}

/*
*   Elevator running down unit test
*/

static bool elevator_running_down_ut(elevator_t *pEle)
{
    printf("Test 4: Running down test...\n");
    bool result = true;
    
    uint8_t entry_floor = 10;
    uint8_t exit_floor = 10;
    
    for(uint8_t i = 0; entry_floor - i > 0; ++i)
    {   
        if(entry_floor == exit_floor - i)
            pickup_passenger(pEle, entry_floor, exit_floor - i);
        else
            pickup_passenger(pEle, entry_floor, exit_floor - i);
    }
    printf("     New passengers picked up : %s\n", (true == result) ? PASSED : FAILED);
    
    for(uint8_t i = 0; i < MAX_FLOORS_COUNT; ++i)
    {
        elevator_step(pEle);
        if(i > 0)
        {
            printf("     #%d Passenger dropped out : ", i);
            result &= test(pEle->pas[i - 1].state == STATE_OUTSIDE);
        }
    }
    printf("Test 4: %s\n\n", (true == result) ? PASSED : FAILED);
    
    return result;
}

/*
*   Elevator unit test function
*/

bool elevator_unit_test()
{
    elevator_t *ele[ELEVATOR_COUNT];
    bool result = true;
    
    printf("========== PERFORMING TEST FOR ELEVATOR ==========\n");
    if(false == (result &= ((ele[0] = elevator_init_ut()) != NULL)));
    if(false == (result &= elevator_pickup_ut(ele[0])));
    if(false == (result &= elevator_running_up_ut(ele[0])));
    if(false == (result &= elevator_running_down_ut(ele[0])));
    
    printf("ELEVATOR TEST : %s\n\n", (true == result) ? PASSED : FAILED);
  return result;
}