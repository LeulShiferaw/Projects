#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>

using namespace std;

mutex m;
queue<string> mQueue;
condition_variable cv;
bool consumer_turn = false;

void producer()
{
    while(true)
    {
        string temp;
        cin >> temp;

        unique_lock<mutex> lck{m};
        mQueue.push(temp);
        
        cv.notify_one();
        consumer_turn = true;
    }
}

void consumer()
{
    while(true)
    {
        unique_lock<mutex> lck{m};
        while(!consumer_turn) cv.wait(lck);

        auto mess = mQueue.front();
        mQueue.pop();
        cout << "Consuming: " << mess << endl;
        consumer_turn = false;
    }
}

int main()
{
    try {
        thread t1(producer);
        thread t2(consumer);

        t1.join();
        t2.join();
    }
    catch(...)
    {
        cout << "Some exception!" << endl;
    }

    return 0;
}