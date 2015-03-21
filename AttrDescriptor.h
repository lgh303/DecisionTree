#ifndef __ATTR_DESCRIPTOR_H__
#define __ATTR_DESCRIPTOR_H__

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class AttrDescriptor
{
public:
	 AttrDescriptor(const string& line, bool is_goal = false)
	 {
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
	 bool goal;
	 string name;
	 bool continuous;
	 std::vector<string> enum_values;

	 void print()
	 {
		  if (continuous)
			   cout << "continuous";
		  else
			   for (int i = 0; i < enum_values.size(); ++i)
					cout << enum_values[i] << " | ";
		  cout << endl;
	 }
		  

private:
	 void parse_str(const string& line)
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
};

#endif // __ATTR_DESCRIPTOR_H__
