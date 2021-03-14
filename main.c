#include "elevator.h"

int main()
{
    elevator_t *ele = create_new_elevator();
    
    pickup_passenger(ele, 3, 7);
    pickup_passenger(ele, 6, 1);
    pickup_passenger(ele, 2, 3);
    pickup_passenger(ele, 1, 4);
    pickup_passenger(ele, 4, 2);
    
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
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    pickup_passenger(ele, 5, 2);
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
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    elevator_step(ele);
    
    print_elevator_status(ele);
    
    pickup_passenger(ele, 2, 3);
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
