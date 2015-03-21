#include <iostream>
#include <string>
#include <vector>
#include "Engine.h"
#include "Node.h"

using namespace std;

//const string attr_descriptor_filename = "data/adult.names";
//const string record_filename = "data/adult.data";
const string attr_descriptor_filename = "test.names";
const string record_filename = "test.data";

string GOAL_0, GOAL_1;

int main()
{
	 Engine* engine = new Engine;
	 engine->init_attr_descriptor(attr_descriptor_filename);
	 engine->init_record(record_filename);
	 engine->build_tree();
	 engine->root->print(0);
	 delete engine;
}
