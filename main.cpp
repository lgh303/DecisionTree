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
const string BLACK_BOX_RESULT_OUT = "2012011303.test.result";

string GOAL_0, GOAL_1;

void out(const string& message, double accur, int size)
{
	 cout << "\t\t" << message;
	 cout << "Accuracy = " << accur << " && ";
	 cout << "Tree Size = " << size << endl;
}

void min_max_avr(double accur, double& min, double& max, double& avr)
{
	 if (accur < min) min = accur;
	 if (accur > max) max = accur;
	 avr += accur;
}

void task(int percent)
{
	 cout  << percent << "% of training set..." << endl;

	 int rounds = 1;
	 if (percent != 100) rounds = 5;

	 double min_before = 1, max_before = 0, avr_before = 0;
	 double min_after = 1, max_after = 0, avr_after = 0;

	 for (int i = 0; i < rounds; ++i)
	 {
		  cout << "\tRound " << i + 1 << endl;
		  double accur;

		  Engine* engine = new Engine;
		  engine->init_attr_descriptor(ATTR_DESCRIPTOR_FILE);
		  engine->load_train_data(TRAIN_FILE, percent);
		  engine->build_tree();

		  accur = engine->test(TRAIN_FILE);
		  out("Before Pruning : ", accur, engine->root->size());
		  min_max_avr(accur, min_before, max_before, avr_before);

		  engine->pessimistic_error_prune(engine->root);

		  accur = engine->test(TRAIN_FILE);
		  out("After  Pruning : ", accur, engine->root->size());
		  min_max_avr(accur, min_after, max_after, avr_after);

		  cout << endl;

		  if (percent == 100)
		  {
			   engine->test_blackbox(BLACK_BOX_FEATURES, BLACK_BOX_RESULT_OUT);
//			   engine->root->print(0);
		  }

		  delete engine;
	 }

	 cout << "\tMaximum : " << endl;
	 cout << "\t\tBefore Pruning : " << max_before <<endl;
	 cout << "\t\tAfter  Pruning : " << max_after <<endl;

	 cout << "\tMinimum : " << endl;
	 cout << "\t\tBefore Pruning : " << min_before <<endl;
	 cout << "\t\tAfter  Pruning : " << min_after <<endl;

	 cout << "\tAverage : " << endl;
	 cout << "\t\tBefore Pruning : " << avr_before / rounds <<endl;
	 cout << "\t\tAfter  Pruning : " << avr_after / rounds <<endl;

	 cout << endl;

	 if (percent == 100)
		  cout << BLACK_BOX_RESULT_OUT << " Generated." << endl;
	 cout << endl;
}

int main(int argc, char** argv)
{
	 srand(time(NULL));
	 
	 task(5);
	 task(50);
	 task(100);
}
