#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "AttrDescriptor.h"

class Node
{
public:
	 Node(AttrDescriptor* ptr, int count_0, int count_1);

	 AttrDescriptor *attr_descriptor;
	 std::vector<Node*> children;

	 // majority of goal-value of the train-records that reach the node.
	 int goal_value;

	 // number of the train-records that reach the node.
	 int count;

	 // number of the train-records whose goal-value is not consistent with the goal-value of the node(wrong judgement). Can be updated.
	 int error_count;

	 // number of errors which occur in leaf nodes. collected upwards when pep-pruning.
	 double leaf_err_collected;

	 ~Node();

	 int size();

	 void print(int indent_count);
};

#endif // __NODE_H__
