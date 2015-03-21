#ifndef __RECORD_H__
#define __RECORD_H__

#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

extern string GOAL_0;

class Record
{
public:
	 Record(const string& line)
	 {
		  int start = 0, end = 0;
		  while (true)
		  {
			   if (line[start] == '.') break;
			   while (line[start] == ',') ++start;
			   end = start + 1;
			   while (line[end] != ',' && line[end] != '.')
					++end;
			   attrs.push_back(line.substr(start, end - start));
			   start = end;
		  }
		  if (attrs.back() == ::GOAL_0)
			   goal = 0;
		  else
			   goal = 1;
		  attrs.pop_back();
	 }
	 void print()
	 {
		  for (int i = 0; i < attrs.size(); ++i)
			   cout << attrs[i] << " | ";
		  cout << endl;
	 }
	 std::vector<std::string> attrs;
	 int goal;
};

#endif //__RECORD_H__
