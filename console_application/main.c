#include <stdio.h>
#include <stdbool.h>
#include "elevator_unit_test.h"


int main()
{
    printf("Running elevator unit test...\n");
    if (true == elevator_unit_test())
		return 0;
    
    return 0;
}



/*

    unsigned int elevator_count = get_elevator_count();
    
    elevator_t *ele[elevator_count];
    
    for(unsigned int i = 0; i < elevator_count; ++i)
    {
        ele[i] = create_new_elevator();
    }
    
    unsigned int option;
    
    while(1)
    {
        printf("ELEVATOR APP\n");
        printf("1. PRINT ELEVATOR STATUS\n");
        printf("2. ADD PASSENGER TO THE ELEVATOR\n");
        printf("3. STEP THE ELEVATOR\n");
        printf("4. EXIT THE APPLICATION\n");
        scanf("%u", &option);
        clear_screen();
        
        switch(option)
        {
            case 1 : 
                printf("WHICH ELEVATOR STATUS DO YOU WANT TO SEE (1 - %u)?\n", elevator_count);
                scanf("%u", &option);
                if((option <= elevator_count) && option)
                    print_elevator_status(ele[option - 1]);
                else
                    printf("INVALID NUMBER\n\n");
                break;
            case 2 :
                printf("TO WHICH ELEVATOR WOULD YOU LIKE TO ADD PASSENGER (1 - %u)?\n", elevator_count);
                scanf("%u", &option);
                if((option <= elevator_count) && option)
                {
                    unsigned int entry_floor, exit_floor;
                    
                    printf("ENTRY FLOOR (1 - %u) :\n", MAX_FLOORS_COUNT);
                    scanf("%u", &entry_floor);
                    printf("EXIT FLOOR (1 - %u) :\n", MAX_FLOORS_COUNT);
                    scanf("%u", &exit_floor);
                    
                    pickup_passenger(ele[option - 1], (uint8_t) entry_floor, (uint8_t) exit_floor);
                }
                else
                    printf("INVALID NUMBER\n\n");
                break;
            case 3 :
                printf("WHICH ELEVATOR WOULD YOU LIKE TO STEP (1 - %u)?\n", elevator_count);
                scanf("%u", &option);
                if((option <= elevator_count) && option)
                    elevator_step(ele[option - 1]);
                break;
            case 4 :
                printf("APPLICATION CLOSING ...\n");
                return 0;
            default :
                printf("INVALID OPTION\n\n");
        }
    }
    
*/