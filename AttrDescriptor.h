#ifndef __ATTR_DESCRIPTOR_H__
#define __ATTR_DESCRIPTOR_H__

#include <string>
#include <vector>

using std::string;

class AttrDescriptor
{
public:
	 AttrDescriptor(const string& line, int _index, bool is_goal = false);
	 bool goal;
	 string name;
	 bool continuous;
	 double threshold;
	 int index;
	 std::vector<string> enum_values;

	 void print();

private:
	 void parse_str(const string& line);
};

#endif // __ATTR_DESCRIPTOR_H__
