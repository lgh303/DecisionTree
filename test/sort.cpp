#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> v_index;
vector<string> v_str;

bool cmp(const int& i, const int& j)
{
	 return atoi(v_str[i].c_str()) < atoi(v_str[j].c_str());
}

int main()
{
	 for (int i = 0; i < 4; ++i)
		  v_index.push_back(i);
	 v_str.push_back("12");
	 v_str.push_back("4");
	 v_str.push_back("9");
	 v_str.push_back("23");
	 v_str.push_back("8");
	 sort(v_index.begin(), v_index.end(), cmp);
	 for (int i = 0; i < v_index.size(); ++i)
		  cout << v_index[i] << " : " << v_str[v_index[i]] << endl;
}
