#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <string>
#include <vector>
#include "Node.h"
#include "AttrDescriptor.h"
#include "Record.h"

using std::vector;

class Engine
{
public:
	 const static int MAX_DEPTH = 4;
	 const static int MAX_CONTINUOUS_REF_TIME = 1;
	 Engine();
	 void init_attr_descriptor(const std::string&);
	 void load_train_data(const std::string&);
	 void build_tree();
	 double test(const std::string&);
	 int predict(Record*);
	 ~Engine();
	 Node* root;
private:
	 std::vector<Record*> records;
	 std::vector<AttrDescriptor*> attr_descriptors;
	 AttrDescriptor* goal_descriptor;
	 Node* build_node(int, const std::vector<int>&, int, int);
	 std::vector<int> attr_set_indexes;
	 int attr_set_count;
	 double entropy(int, int);
	 double calc_new_entropy(const vector< vector<int> >&, int, vector<int>&, vector<int>&);

};

#endif //__ENGINE_H__
