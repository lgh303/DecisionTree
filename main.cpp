#include <iostream>
#include <string>
#include <vector>
#include "Engine.h"
#include "Node.h"

using namespace std;

const string attr_descriptor_filename = "data/adult.names";
const string record_filename = "data/adult.names";

int main()
{
	 Engine* engine = new Engine;
	 engine->init_attr_descriptor(attr_descriptor_filename);
	 engine->init_record(record_filename);
	 vector<int> record_indexes;
	 Node* root = engine->build_node(record_indexes);
	 delete root;
	 delete engine;
}
