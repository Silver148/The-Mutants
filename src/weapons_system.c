#include "weapons_system.h"
#include "projectiles.h"

int Ammunition = 100; // initial player ammunition (unchanged)

bool CheckMunitions(){
    if(Ammunition <= 0){
        return false;
    }

    return true;
}
