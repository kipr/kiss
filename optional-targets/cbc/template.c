/***************************************************************************/
/*********     This comment should be filled with a description    *********/
/*********         of your program and what it does.               *********/
/*********       This template is for Botball programs.            *********/
/*********    Make sure you have selected the CBC target.          *********/
/***************************************************************************/

/* Libraries used in Botball are automatically included, but list any additional includes here */

/* #defines and constants go here.*/

/*Global variables go here (if you absolutely need them).*/

/*Function prototypes below*/

int main()
{
    //All variable declarations go here.
    
    //This is your main function.
    
    //Wait for starting light with light sensor in the_port_number
        wait_for_light(the_port_number);
        
    //Run your_code for the amount_of_time
        run_for(amount_of_time, your_code);
        
    //Make sure your motors are stopped
        ao();
        create_stop();
        
    //Put anything below you want your robot to do after the game (e.g. play victory song).
    
}

/*Declare functions here as needed.*/
