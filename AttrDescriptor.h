#ifndef __ATTR_DESCRIPTOR_H__
#define __ATTR_DESCRIPTOR_H__

#include <string>
#include <vector>

class AttrDescriptor
{
public:
	 std::string name;
	 bool continous;
	 std::vector<std::string> enum_values;
};

#endif // __ATTR_DESCRIPTOR_H__
