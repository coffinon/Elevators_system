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
*   Elevator initialization unit test function
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
            printf("     #%d Passenger has unique ID: ", i);
            result &= test(pEle->pas[i].id == i + 1);
            printf("     #%d Passenger entry floor: ", i);
            result &= test(pEle->pas[i].entryFloor == 0);
            printf("     #%d Passenger exit floor: ", i);
            result &= test(pEle->pas[i].exitFloor == 0);
            printf("     #%d Passenger state: ", i);
            result &= test(pEle->pas[i].state == STATE_OUTSIDE);
        }
    }
    
    printf("Test 1: %s\n\n", (true == result) ? PASSED : FAILED);
    return pEle;
}

/*
*   Elevator functionality unit test function
*/

static bool elevator_test_ut(elevator_t *pEle)
{
    printf("Test 2: Elevator go up test...\n");
    bool result = true;
    
    uint8_t entry_floor = 1;
    uint8_t exit_floor = 1;
    
    for(uint8_t i = 0; entry_floor + i <= MAX_FLOORS_COUNT; ++i)
    {   
        if((entry_floor == exit_floor + i))
        {
            printf("     #%d Passenger request not picked up : ", i);
            result &= !test(pickup_passenger(pEle, entry_floor, exit_floor + i));
        }
        else
        {
            printf("     #%d Passenger request picked up : ", i);
            result &= test(pickup_passenger(pEle, entry_floor, exit_floor + i));
        }
    }
    printf("New passengers picked up : %s\n\n", (true == result) ? PASSED : FAILED);
    
    
    for(uint8_t i = 0; i < MAX_FLOORS_COUNT; ++i)
    {
        elevator_step(pEle);
        if(i > 0)
        {
            printf("     #%d Passenger dropped out : ", i - 1);
            result &= test(pEle->pas[i - 1].state == STATE_OUTSIDE);
        }
    }
    printf("Test 2: %s\n\n", (true == result) ? PASSED : FAILED);
    
    printf("Test 3: Elevator go down test...\n");
    
    entry_floor = 10;
    exit_floor = 10;
    
    for(uint8_t i = 0; entry_floor - i > 0; ++i)
    {   
        if((entry_floor == exit_floor - i))
        {
            printf("     #%d Passenger request not picked up : ", i);
            result &= !test(pickup_passenger(pEle, entry_floor, exit_floor - i));
        }
        else
        {
            printf("     #%d Passenger request picked up : ", i);
            result &= test(pickup_passenger(pEle, entry_floor, exit_floor - i));
        }
    }
    printf("New passengers picked up : %s\n\n", (true == result) ? PASSED : FAILED);
    
    for(uint8_t i = 0; i < MAX_FLOORS_COUNT; ++i)
    {
        elevator_step(pEle);
        if(i > 0)
        {
            printf("     #%d Passenger dropped out : ", i - 1);
            result &= test(pEle->pas[i - 1].state == STATE_OUTSIDE);
        }
    }
    printf("Test 3: %s\n\n", (true == result) ? PASSED : FAILED);
    
    
    
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
    //for(uint8_t i = 0; true == result && i < ELEVATOR_COUNT; ++i) {
    //    do {
            if(false == (result &= (((ele[0] = elevator_init_ut()) == NULL) ? false : true)));//break;
            if(false == (result &= elevator_test_ut(ele[0])));//break;
    //    } while(0);
    //    printf("========== TEST FOR ELEVATOR %d FINISHED WITH RESULT %s ==========\n", i, (true == result) ? PASSED : FAILED);
    //}
  return result;
}