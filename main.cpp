#include <iostream>
#include <string>
#include <vector>
#include "Engine.h"
#include "Node.h"

using namespace std;

const string attr_descriptor_filename = "data/adult.names";
const string train_filename = "data/adult.data";
const string test_filename = "data/adult.test";
//const string attr_descriptor_filename = "test.names";
//const string record_filename = "test.data";

string GOAL_0, GOAL_1;

int main()
{
	 Engine* engine = new Engine;
	 engine->init_attr_descriptor(attr_descriptor_filename);
	 engine->load_train_data(train_filename);
	 engine->build_tree();
//	 engine->root->print(0);
	 double accur = engine->test(test_filename);
	 cout << accur << endl;
	 delete engine;
}
