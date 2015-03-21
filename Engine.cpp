#include "Engine.h"
#include "Record.h"
#include "AttrDescriptor.h"
#include "Node.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

extern string GOAL_0;
extern string GOAL_1;

Engine::Engine()
{
	 goal_descriptor = NULL;
	 root = NULL;
}

void Engine::init_attr_descriptor(const string& filename)
{
	 ifstream fin(filename.c_str());
	 string line;
	 while (fin)
	 {
		  getline(fin, line);
		  if (line[0] == '|') continue;
		  if (line.size() <= 0) continue;
		  if (!goal_descriptor)
		  {
			   goal_descriptor = new AttrDescriptor(line, true);
			   GOAL_0 = goal_descriptor->enum_values[0];
			   GOAL_1 = goal_descriptor->enum_values[1];
			   continue;
		  }
		  AttrDescriptor* desc = new AttrDescriptor(line);
		  attr_descriptors.push_back(desc);
		  attr_set_indexes.push_back(1);
	 }
	 fin.close();
	 attr_set_count = attr_set_indexes.size();
}

void Engine::init_record(const string& filename)
{
	 ifstream fin(filename.c_str());
	 string line;
	 while (fin)
	 {
		  getline(fin, line);
		  if (line.size() <= 0) continue;
		  Record* rec = new Record(line);
		  records.push_back(rec);
	 }
	 fin.close();
}

void Engine::build_tree()
{
	 vector<int> record_indexes;
	 for (int i = 0; i < records.size(); ++i)
		  record_indexes.push_back(i);
	 int count_0 = 0, count_1 = 0;
	 for (int i = 0; i < records.size(); ++i)
		  if (records[i]->goal == 0)
			   ++count_0;
		  else
			   ++count_1;
	 root = build_node(record_indexes, count_0, count_1);
}

Node* Engine::build_node(const vector<int> &record_indexes, int count_0, int count_1)
{
	 if (count_0 == 0)
	 {
		  Node *node = new Node(goal_descriptor);
		  node->goal_value = 1;
		  return node;
	 }
	 if (count_1 == 0)
	 {
		  Node *node = new Node(goal_descriptor);
		  node->goal_value = 0;
		  return node;
	 }
	 if (attr_set_count == 0)
	 {
		  Node *node = new Node(goal_descriptor);
		  if (count_0 > count_1)
			   node->goal_value = 0;
		  else
			   node->goal_value = 1;
		  return node;
	 }

	 double max_gain = 0;
	 int chosen_attr_index = -1;
	 vector< vector<int> > index_sets_save;
	 vector<int> count_0_sets_save, count_1_sets_save;

	 for (int i = 0; i < attr_set_indexes.size(); ++i)
		  if (attr_set_indexes[i] == 1)
		  {
			   AttrDescriptor* attr_desc_ptr = attr_descriptors[i];
			   vector< vector<int> > index_sets;
			   vector<int> count_0_sets, count_1_sets;
			   if (attr_desc_ptr->continuous)
			   {
					assert(false);
			   }
			   else
			   {
					for (int j = 0; j < attr_desc_ptr->enum_values.size(); ++j)
					{
						 string enum_value = attr_desc_ptr->enum_values[j];
						 vector<int> index_set;
						 for (int k = 0; k < record_indexes.size(); ++k)
							  if (records[record_indexes[k]]->attrs[i] == enum_value)
								   index_set.push_back(record_indexes[k]);
						 index_sets.push_back(index_set);
					}
					
			   }

			   double new_entropy = 0;
			   int record_size = record_indexes.size();
			   for (int j = 0; j < index_sets.size(); ++j)
			   {
					int partial_size = index_sets[j].size();
					int partial_count_0 = 0, partial_count_1 = 0;
					for (int k = 0; k < partial_size; ++k)
						 if (records[index_sets[j][k]]->goal == 0)
							  ++partial_count_0;
						 else
							  ++partial_count_1;
					double ratio = (double)partial_size / (double)record_size;
					new_entropy += ratio * entropy(partial_count_0, partial_count_1);
					count_0_sets.push_back(partial_count_0);
					count_1_sets.push_back(partial_count_1);
			   }

			   double gain = entropy(count_0, count_1) - new_entropy;
			   if (gain > max_gain)
			   {
					max_gain = gain;
					chosen_attr_index = i;
					index_sets_save = index_sets;
					count_0_sets_save = count_0_sets;
					count_1_sets_save = count_1_sets;
			   }
		  }

	 assert(chosen_attr_index != -1);
	 
	 attr_set_indexes[chosen_attr_index] = 0;
	 attr_set_count--;

	 Node* node = new Node(attr_descriptors[chosen_attr_index]);
	 for (int i = 0; i < node->attr_descriptor->enum_values.size(); ++i)
	 {
		  Node* child = build_node(index_sets_save[i], count_0_sets_save[i], count_1_sets_save[i]);
		  node->children.push_back(child);
	 }

	 return node;
}

Engine::~Engine()
{
	 if (root)
		  delete root;
	 if (goal_descriptor)
		  delete goal_descriptor;
	 for (int i = 0; i < records.size(); ++i)
		  delete records[i];
	 for (int i = 0; i < attr_descriptors.size(); ++i)
		  delete attr_descriptors[i];
}

double Engine::entropy(int count_0, int count_1)
{
	 if (count_0 == 0 || count_1 == 0) return 0;
	 int total = count_0 + count_1;
	 double p0 = (double)count_0 / (double)total;
	 double p1 = (double)count_1 / (double)total;
	 return (-p0 * log(p0) - p1 * log(p1)) / log(2.0);
}
