#include "ttt.h"
#include "testing.h"
#include <iostream>

using namespace std;

int main()
{
	try
	{
		
		TTT ttt;
		ttt.play_ai_game();
		/*Testing*/
		/*
		test_max_min();
		*/
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