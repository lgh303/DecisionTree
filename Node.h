#ifndef __NODE_H__
#define __NODE_H__

#include <string>
#include <vector>
#include "AttrDescriptor.h"

#include <iostream>

extern std::string GOAL_0;
extern std::string GOAL_1;

class Node
{
public:
	 Node(AttrDescriptor* ptr, int count_0, int count_1)
	 {
		  attr_descriptor = ptr;
		  count = count_0 + count_1;
		  if (count_0 > count_1)
		  {
			   goal_value = 0;
			   error_count = count_1;
		  }
		  else
		  {
			   goal_value = 1;
			   error_count = count_0;
		  }
		  leaf_err_collected = error_count + 0.5;
	 }

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

	 ~Node()
	 {
		  for (int i = 0; i < children.size(); ++i)
			   delete children[i];
	 }

	 int size()
	 {
		  if (attr_descriptor->goal)
			   return 1;
		  int ret = 0;
		  for (int i = 0; i < children.size(); ++i)
			   ret += children[i]->size();
		  return ret;
	 }

	 void print(int indent_count)
	 {
		  for (int i = 0; i < indent_count; ++i)
			   std::cout << '\t';
		  if (attr_descriptor->goal)
		  {
			   if (goal_value == 0)
					std::cout << GOAL_0 << std::endl;
			   else
					std::cout << GOAL_1 << std::endl;
		  }
		  else
			   std::cout << '(' << attr_descriptor->name << ')' << std::endl;

		  if (attr_descriptor->continuous)
		  {
			   for (int i = 0; i < indent_count; ++i)
					std::cout << '\t';
			   std::cout << "<-" << attr_descriptor->threshold << std::endl;
			   children[0]->print(indent_count + 1);
			   for (int i = 0; i < indent_count; ++i)
					std::cout << '\t';
			   std::cout << attr_descriptor->threshold << "->" << std::endl;
			   children[1]->print(indent_count + 1);
		  }
		  else
		  {
			   for (int i = 0; i < children.size(); ++i)
			   {
					for (int j = 0; j < indent_count; ++j)
						 std::cout << '\t';
					std::cout << "<-" << attr_descriptor->enum_values[i] << "->" << std::endl;
					children[i]->print(indent_count + 1);
			   }
		  }
	 }
};

#endif // __NODE_H__
