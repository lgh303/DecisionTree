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
	 void build_tree();
	 ~Engine();
	 Node* root;
private:
	 std::vector<Record*> records;
	 std::vector<AttrDescriptor*> attr_descriptors;
	 AttrDescriptor* goal_descriptor;
	 Node* build_node(const std::vector<int>&, int, int);
	 std::vector<int> attr_set_indexes;
	 int attr_set_count;
	 double entropy(int, int);
};

#endif //__ENGINE_H__
