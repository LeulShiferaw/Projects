#include "game.h"
#include <iostream>

using namespace std;

int main()
{  
    try {
        Game g;
        g.GameLoop();
    }
    catch(...)
    {
        cerr << "Some exception" << endl;
    }
    return 0;
}