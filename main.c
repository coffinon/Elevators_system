#include <stdio.h>
#include "elevator.h"

int main()
{
    elevator_t *ele = create_new_elevator();
    
    print_elevator_status(ele);
    
    passenger_t *pas = create_new_passenger(3, 4);
    
    add_passenger(ele, pas);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    passenger_t *pas2 = create_new_passenger(3, 2);
    
    add_passenger(ele, pas2);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);


    return 0;
}
