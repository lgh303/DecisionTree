#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <string>
#include <vector>
#include "Node.h"
#include "AttrDescriptor.h"
#include "Record.h"

class Engine
{
public:
	 Engine();
	 void init_attr_descriptor(const std::string&);
	 void init_record(const std::string&);
	 Node* build_node(std::vector<int>&);
	 ~Engine();
private:
	 std::vector<Record*> records;
	 std::vector<AttrDescriptor*> attr_descriptors;
	 AttrDescriptor goal_descriptor;
};

#endif //__ENGINE_H__
