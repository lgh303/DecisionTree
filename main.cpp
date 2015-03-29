#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Engine.h"
#include "Node.h"

using namespace std;

const string ATTR_DESCRIPTOR_FILE = "data/adult.names";

const string TRAIN_FILE = "data/adult.data";
const string TEST_FILE = "data/adult.test";
const string BLACK_BOX_FEATURES = "data/test.features";
const string BLACK_BOX_RESULT_STD = "data/test.result.example";
const string BLACK_BOX_RESULT_OUT = "2012011303.result.example";

string GOAL_0, GOAL_1;

int main(int argc, char** argv)
{
	 if (argc < 2)
	 {
		  cerr << "Usage : ./main OPTION" << endl;
		  cerr << "options:" << endl;
		  cerr << "\t--test : use adult.test to test" << endl;
		  cerr << "\t--train : use adult.train to test" << endl;
		  return 0;
	 }

	 string test_filename;
	 if (string(argv[1]) == "--test")
		  test_filename = TEST_FILE;
	 else if (string(argv[1]) == "--train")
		  test_filename = TRAIN_FILE;
	 else
	 {
		  cerr << "invalid option" << endl;
		  return 0;
	 }

	 srand(time(NULL));
	 Engine* engine = new Engine;
	 engine->init_attr_descriptor(ATTR_DESCRIPTOR_FILE);
	 engine->load_train_data(TRAIN_FILE);
	 engine->build_tree();
//	 engine->root->print(0);

	 double accur = engine->test(test_filename);
	 cout << accur << ' ' << engine->root->size() << endl;

	 engine->pessimistic_error_prune(engine->root);

	 accur = engine->test(test_filename);
	 cout << accur << ' ' << engine->root->size() << endl;

	 delete engine;
}
