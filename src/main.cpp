#include "tw_game.h"

#include <iostream>
#include <game.h>
#include <exception.h>
#include <engine.h>
#include <events_translator.h>

using namespace ijengine;
using namespace std;

int main(){

    try {
        TWGame game("Traveling Will", 852, 480);
        game.run("menu");
    }
    catch (Exception ex){
        cout << ex.what() << endl;
    }

    return 0;
}
