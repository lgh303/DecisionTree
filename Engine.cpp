#include "Engine.h"
#include "Record.h"
#include "AttrDescriptor.h"
#include "Node.h"
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

extern string GOAL_0;
extern string GOAL_1;

vector<int> sort_index_keys;
bool compare(const int& i, const int& j)
{
	 return sort_index_keys[i] < sort_index_keys[j];
}

Engine::Engine()
{
	 goal_descriptor = NULL;
	 root = NULL;
}

void Engine::init_attr_descriptor(const string& filename)
{
	 ifstream fin(filename.c_str());
	 string line;
	 int count = -1;
	 while (fin)
	 {
		  getline(fin, line);
		  if (line[0] == '|') continue;
		  if (line.size() <= 0) continue;
		  if (!goal_descriptor)
		  {
			   goal_descriptor = new AttrDescriptor(line, -1, true);
			   GOAL_0 = goal_descriptor->enum_values[0];
			   GOAL_1 = goal_descriptor->enum_values[1];
			   continue;
		  }
		  ++count;
		  AttrDescriptor* desc = new AttrDescriptor(line, count);
		  attr_descriptors.push_back(desc);
		  attr_set_indexes.push_back(1);
	 }
	 fin.close();
	 attr_set_count = attr_set_indexes.size();
}

void Engine::load_train_data(const string& filename)
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
	 root = build_node(0, record_indexes, count_0, count_1);
}

Node* Engine::build_node(int depth, const vector<int> &record_indexes, int count_0, int count_1)
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
	 if (attr_set_count == 0 || depth == MAX_DEPTH)
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
	 double threshold_save;

	 for (int i = 0; i < attr_set_indexes.size(); ++i)
		  if (attr_set_indexes[i] == 1)
		  {
			   AttrDescriptor* attr_desc_ptr = attr_descriptors[i];
			   vector< vector<int> > index_sets;
			   vector<int> count_0_sets, count_1_sets;
			   double threshold = -1;
			   double best_entropy = 1e100;

			   if (attr_desc_ptr->continuous)
			   {
					sort_index_keys.clear();
					vector<int> record_indexes_sorted;
					for (int j = 0; j < record_indexes.size(); ++j)
					{
						 string attr = records[record_indexes[j]]->attrs[i];
						 if (attr != "?")
						 {
							  sort_index_keys.push_back(atoi(attr.c_str()));
							  record_indexes_sorted.push_back(record_indexes[j]);
						 }
					}
					sort(record_indexes_sorted.begin(), record_indexes_sorted.end(), compare);
					
					int refresh_count = 0;
					for (int j = 1; j < record_indexes_sorted.size(); ++j)
					{
						 if (records[record_indexes_sorted[j]]->goal != records[record_indexes_sorted[j - 1]]->goal)
						 {
							  double threshold_tmp = 0.5 * (sort_index_keys[j] + sort_index_keys[j - 1]);
							  vector< vector<int> > index_sets_tmp;
							  vector<int> count_0_sets_tmp, count_1_sets_tmp;
							  vector<int> index_set_1, index_set_2;
							  for (int k = 0; k < j; ++k)
								   index_set_1.push_back(record_indexes_sorted[k]);
							  for (int k = j; k < record_indexes_sorted.size(); ++k)
								   index_set_2.push_back(record_indexes_sorted[k]);
							  index_sets_tmp.push_back(index_set_1);
							  index_sets_tmp.push_back(index_set_2);

							  double new_entropy = calc_new_entropy(index_sets_tmp, record_indexes_sorted.size(), count_0_sets_tmp, count_1_sets_tmp);
							  if (new_entropy < best_entropy)
							  {
								   best_entropy = new_entropy;
								   threshold = threshold_tmp;
								   index_sets = index_sets_tmp;
								   count_0_sets = count_0_sets_tmp;
								   count_1_sets = count_1_sets_tmp;
								   ++refresh_count;
								   if (refresh_count > MAX_CONTINUOUS_REF_TIME)
										break;
							  }
						 }
					}
			   }
			   else
			   {
					int max_enum_val_index = -1;
					int max_enum_val_count = -1;
					for (int j = 0; j < attr_desc_ptr->enum_values.size(); ++j)
					{
						 int enum_val_count = 0;
						 for (int k = 0; k < record_indexes.size(); ++k)
							  if (records[record_indexes[k]]->attrs[i] == attr_desc_ptr->enum_values[j])
								   enum_val_count++;
						 if (enum_val_count > max_enum_val_count)
						 {
							  max_enum_val_count = enum_val_count;
							  max_enum_val_index = j;
						 }
					}

					for (int j = 0; j < attr_desc_ptr->enum_values.size(); ++j)
					{
						 string enum_value = attr_desc_ptr->enum_values[j];
						 vector<int> index_set;
						 for (int k = 0; k < record_indexes.size(); ++k)
							  if (records[record_indexes[k]]->attrs[i] == enum_value
								  || (records[record_indexes[k]]->attrs[i] == "?" 
									  && j == max_enum_val_index))
								   index_set.push_back(record_indexes[k]);
						 index_sets.push_back(index_set);
					}

					best_entropy = calc_new_entropy(index_sets, record_indexes.size(), count_0_sets, count_1_sets);
			   }

			   double gain = entropy(count_0, count_1) - best_entropy;
			   if (gain > max_gain)
			   {
					max_gain = gain;
					chosen_attr_index = i;
					threshold_save = threshold;
					index_sets_save = index_sets;
					count_0_sets_save = count_0_sets;
					count_1_sets_save = count_1_sets;
			   }
		  }

	 assert(chosen_attr_index != -1);
	 
	 Node* node = new Node(attr_descriptors[chosen_attr_index]);
	 if (node->attr_descriptor->continuous)
		  node->threshold = threshold_save;
	 for (int i = 0; i < index_sets_save.size(); ++i)
	 {
		  attr_set_indexes[chosen_attr_index] = 0;
		  attr_set_count--;
		  Node* child = build_node(depth + 1, index_sets_save[i], count_0_sets_save[i], count_1_sets_save[i]);
		  attr_set_count++;
		  attr_set_indexes[chosen_attr_index] = 1;
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

double Engine::calc_new_entropy(const vector< vector<int> > &index_sets, int record_size, vector<int>& count_0_sets, vector<int>& count_1_sets)
{
	 double new_entropy = 0;
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
	 return new_entropy;
}

double Engine::test(const std::string& filename)
{
	 int total_count = 0, accur_count = 0;
	 ifstream fin(filename.c_str());
	 string line;
	 while (fin)
	 {
		  getline(fin, line);
		  if (line.size() <= 0) continue;
		  ++total_count;
		  Record* rec = new Record(line);
		  if (predict(rec) == rec->goal)
			   ++accur_count;
		  delete rec;
	 }
	 fin.close();
	 return (double)accur_count / (double)total_count;
}

int Engine::predict(Record* record)
{
	 Node *node = root;
	 while (true)
	 {
		  AttrDescriptor* attr_desc = node->attr_descriptor;
		  if (attr_desc->goal)
			   return node->goal_value;
		  string attr = record->attrs[attr_desc->index];
		  if (attr_desc->continuous)
		  {
			   int x = atoi(attr.c_str());
			   if (x == 0 && attr != "0") assert(attr == "?");
			   if (x < node->threshold)
					node = node->children[0];
			   else
					node = node->children[1];
		  }
		  else
		  {
			   bool found = false;
			   for (int i = 0; i < attr_desc->enum_values.size(); ++i)
					if (attr == attr_desc->enum_values[i])
					{
						 node = node->children[i];
						 found = true;
						 break;
					}
			   if (!found)
			   {
					assert(attr == "?");
					node = node->children[0];
			   }
		  }
	 }
}
