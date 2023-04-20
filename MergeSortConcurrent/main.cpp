#include <vector>
#include <iostream>
#include <future>
#include <chrono>
#include <algorithm>

using namespace std;

vector<int> nums;
void merge_sort(int s, int e)
{
	if(e == s+1) return;

	int m = (s+e)/2;

    /*
    thread t1(merge_sort, s, m);
    thread t2(merge_sort, m, e);

    t1.join();
    t2.join();
    */

    merge_sort(s, m);
    merge_sort(m, e);
	vector<int> new_vec(e-s);
	merge(nums.begin()+s, nums.begin()+m, nums.begin()+m, nums.begin()+e, new_vec.begin());
	copy(new_vec.begin(), new_vec.end(), nums.begin()+s);
}

int main()
{
    int n;
    cin >> n;
    for(int i = 1; i<=n; ++i) nums.push_back(i);

    auto t0 = chrono::high_resolution_clock::now();
    /*
    thread th1(merge_sort, 0, nums.size()/2);
    thread th2(merge_sort, nums.size()/2, nums.size());

    th1.join();
    th2.join();
    */
    merge_sort(0, nums.size());
    auto t1 = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(t1-t0).count() << "ms\n";
}