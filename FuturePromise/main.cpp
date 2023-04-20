#include <future>
#include <thread>
#include <iostream>

using namespace std;

future<int> fut;
promise<int> prom;

void f()
{
    try {
        cout << "I am in f" << endl;
        prom.set_value(4);
    }
    catch(...)
    {
        cerr << "Some error in f!" << endl;
    }
}

void g()
{
    try {
        cout << fut.get() << endl;
    }
    catch(...)
    {
        cerr << "Some error in g!" << endl;
    }
}

int main()
{
    try {
        thread t1(f);
        thread t2(g);

        t1.join();
        t2.join();
    }
    catch(...)
    {
        cout << "Some exception!" << endl;
    }

    return 0;
}