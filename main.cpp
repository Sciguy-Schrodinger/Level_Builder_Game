#include "game.h"
#include "Main_Menu.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[]) {
    srand(time(0));
     while(true){ 
        int result = Main_Menu(argc, argv); 
        
        if(result == 0){  
            break;
        }
        
        run_game();  
    }
    
    return 0;
}

