#ifndef __NODE_H__
#define __NODE_H__

#include <vector>

class Node
{
public:
	 int attr_desc_index;
	 std::vector<Node*> children;
};

#endif // __NODE_H__
