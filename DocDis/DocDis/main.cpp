#include <map>
#include <fstream>
#include <iostream>

using namespace std;

string doc1 = "doc1.txt";
string doc2 = "doc2.txt";

map<string, int> word_freq1;
map<string, int> word_freq2;

double magnitude(const map<string, int>& word_freq)
{
	long long res = 0;
	for (auto itr = word_freq.begin(); itr != word_freq.end(); ++itr)
	{
		res += itr->second * itr->second;
	}
	return sqrt(res);
}

long long dot_pro()
{
	long long res = 0;
	for (auto itr = word_freq1.begin(); itr != word_freq1.end(); ++itr)
	{
		if (word_freq2.find(itr->first) != word_freq2.end())
		{
			res += itr->second * word_freq2[itr->first];
		}
	}
	return res;
}

int main()
{
	ifstream file1(doc1);
	ifstream file2(doc2);

	string str;
	while (file1 >> str)
	{
		auto ele = word_freq1.find(str);
		if (ele == word_freq1.end())
			word_freq1.insert(make_pair(str, 1));
		else
			++word_freq1[str];
	}

	while (file2 >> str)
	{
		auto ele = word_freq2.find(str);
		if (ele == word_freq2.end())
			word_freq2.insert(make_pair(str, 1));
		else
			++word_freq2[str];
	}

	file1.close();
	file2.close();


	double res = dot_pro() / (double)(magnitude(word_freq1) * magnitude(word_freq2));
	cout << word_freq1["dog"] << endl;
	cout << res * 100 << endl;

	return 0;
}