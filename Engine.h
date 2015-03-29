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
	 const static int MAX_DEPTH = 100;

	 Node* root;

	 Engine();

	 void init_attr_descriptor(const std::string&);
	 void load_train_data(const std::string&, int);

	 void build_tree();
	 void pessimistic_error_prune(Node*);

	 double test(const std::string&);
	 void test_blackbox(const std::string&, const std::string&);
	 double compare_result(const std::string&, const std::string&);

	 int predict(Record*);

	 ~Engine();

private:

	 std::vector<Record*> records;

	 AttrDescriptor* goal_descriptor;
	 std::vector<AttrDescriptor*> attr_descriptors;

	 std::vector<int> attr_set_indexes;
	 int attr_set_count;

	 double entropy(int, int);
	 double calc_new_entropy(const vector< vector<int> >&, int, vector<int>&, vector<int>&, double&);
	 Node* build_node(int, const std::vector<int>&, int, int);

	 void set_threshold();
	 bool on_prune_condition(Node*);
};

#endif //__ENGINE_H__
