#include "ttt.h"
#include <iostream>

using namespace std;

int main()
{
	try
	{
		TTT ttt;
		ttt.play_game();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		cout << "Exception found!" << endl;
		return 2;
	}
}