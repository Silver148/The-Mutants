#include "weapons_system.h"
#include "projectiles.h"

int Ammunition = 100; //100 bullets for now xD

bool CheckMunitions(){
    if(Ammunition <= 0){
        return false;
    }

    return true;
}
