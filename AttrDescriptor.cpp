#include "AttrDescriptor.h"
#include <string>
#include <iostream>

using namespace std;

AttrDescriptor::AttrDescriptor(const string& line, int _index, bool is_goal)
{
	 index = _index;
	 goal = is_goal;
	 if (is_goal)
	 {
		  name = "GOAL";
		  continuous = false;
		  parse_str(line);
	 }
	 else
	 {
		  string rest_str;
		  for (int i = 0; i < line.size(); ++i)
		  {
			   if (line[i] == ':')
			   {
					name = line.substr(0, i);
					rest_str = line.substr(i + 1, line.size() - i - 1);
					break;
			   }
		  }
		  if (rest_str.find("continuous") != string::npos)
			   continuous = true;
		  else
		  {
			   continuous = false;
			   parse_str(rest_str);
		  }
	 }
}

void AttrDescriptor::print()
{
	 if (continuous)
		  cout << "continuous";
	 else
		  for (int i = 0; i < enum_values.size(); ++i)
			   cout << enum_values[i] << " | ";
	 cout << endl;
}

void AttrDescriptor::parse_str(const string& line)
{
	 int start = 0, end = 0;
	 while (true)
	 {
		  if (line[start] == '.') break;
		  while (line[start] == ',' || line[start] == ' ')
			   ++start;
		  end = start + 1;
		  while (line[end] != ',' && line[end] != ' ' && line[end] != '.')
			   ++end;
		  enum_values.push_back(line.substr(start, end - start));
		  start = end;
	 }
}
