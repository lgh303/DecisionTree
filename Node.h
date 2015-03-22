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
	 Node(AttrDescriptor* ptr = NULL)
	 {
		  attr_descriptor = ptr;
	 }
	 AttrDescriptor *attr_descriptor;
	 std::vector<Node*> children;
	 double threshold; // used for continuous attr
	 int goal_value; // used for goal attr
	 int count;
	 ~Node()
	 {
		  for (int i = 0; i < children.size(); ++i)
			   delete children[i];
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
			   std::cout << "<-" << threshold << std::endl;
			   children[0]->print(indent_count + 1);
			   for (int i = 0; i < indent_count; ++i)
					std::cout << '\t';
			   std::cout << threshold << "->" << std::endl;
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
