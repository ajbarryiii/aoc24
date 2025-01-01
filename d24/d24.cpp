#include <cassert>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <regex>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <unordered_set>

using namespace std;

pair<unordered_map<string,bool>, deque<vector<string>>> parse_file (string filename) {
	unordered_map<string,bool> map;
	ifstream file(filename);
	assert(file.is_open());
	string line;
	while (getline(file,line)) {
		if (line.empty()) {
			break;
		}
		string key = line.substr(0,line.find_first_of(':'));
		int value = stoi(line.substr(line.find_first_of(':')+1));
		map[key] = (value==0) ? false:true;
	}

	deque<vector<string>> ops;
	regex pattern("(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+->\\s+(\\w+)");
	while (getline(file,line)) {
		smatch match;
		vector<string> curr(4);
		regex_search(line, match, pattern);
		curr[0] = match[1];
		curr[1] = match[3];
		curr[2] = match[4];
		curr[3] = match[2];
		//each queue element will be structured [first, second, out, operation]
		ops.push_back(curr);
	}
	return {map,ops};
}

// idea: use a deque to order ops, if op is not valid, move to back of the queue

unordered_map<string,bool> get_result_map (pair<unordered_map<string,bool>, deque<vector<string>>> input) {
	unordered_map<string,bool> map = input.first;
	deque<vector<string>> q = input.second;
	while(!q.empty()) {
		vector<string> curr = q.front();
		q.pop_front();
		if (map.contains(curr[0]) && map.contains(curr[1])) {
			string op = curr[3];
			if (op == "AND") {
				map[curr[2]] = map[curr[0]] && map[curr[1]];
			}
			else if (op == "OR") {
				map[curr[2]] = map[curr[0]] || map[curr[1]];
			}
			else if (op == "XOR") {
				map[curr[2]] = map[curr[0]] ^ map[curr[1]];
			}
			else {
				cout << "Unrecognized operation: " << op << '\n';
				assert(false);
			}
		}
		else {
			q.push_back(curr);
		}
	}
	return map;
}


long get_result (pair<unordered_map<string,bool>, deque<vector<string>>> input) {
	unordered_map<string,bool> map = get_result_map(input);
	long result = 0;
	for (auto key: map) {
		if (key.first[0]=='z') {
			long idx = stol(key.first.substr(1));
			if (key.second) {
				result += (1L<<idx);
			}
		}
		assert (result>=0);
	}
	return result;
}

bool get_bit_i (long num, long i) {
	bool bit_i = (num >> i) & 1L; 
	return bit_i;
}

enum Operation {
	AND,
	OR,
	XOR,
	PRESET_T,
	PRESET_F
};

struct TreeNode {
	string out;
	Operation operation;
	TreeNode* right_input;
	TreeNode* left_input;
	TreeNode(string out, Operation op): out(out), operation(op), right_input(nullptr), left_input(nullptr) {};
	TreeNode(string out, Operation op, TreeNode* right_input, TreeNode* left_input): out(out), operation(op), right_input(right_input), left_input(left_input) {};
	bool get_value () const {
		if (operation == Operation::PRESET_F) {
			return false;
		}
		else if (operation == Operation::PRESET_T) {
			return true;
		}
		else if (operation == Operation::XOR) {
			return right_input->get_value() ^ left_input->get_value();
		}
		else if (operation == Operation::AND) {
			return right_input->get_value() && left_input->get_value();
		}
		else if (operation == Operation::OR) {
			return right_input->get_value() || left_input->get_value();
		}
		else {
			cout << "unrecognized operation";
			assert(false);
		}
	}
	unordered_set<string> get_components_subset() {
		unordered_set<string> result;
		if (operation==Operation::PRESET_T || operation==Operation::PRESET_F) {
			return result;
		}
		vector<TreeNode*> q;
		q.push_back(right_input);
		q.push_back(left_input);
		while (!q.empty()) {
			TreeNode* curr = q.back();
			q.pop_back();
			if (!result.contains(curr->out)) {
				result.insert(curr->out);
				if (curr->left_input!=nullptr) q.push_back(curr->left_input);
				if (curr->right_input!=nullptr) q.push_back(curr->right_input);
			}
		}
		return result;
	}
};

pair<unordered_map<string,TreeNode>, deque<vector<string>>> parse_file_p2 (string filename) {
	unordered_map<string,TreeNode> map;
	ifstream file(filename);
	assert(file.is_open());
	string line;
	while (getline(file,line)) {
		if (line.empty()) {
			break;
		}
		string key = line.substr(0,line.find_first_of(':'));
		int value = stoi(line.substr(line.find_first_of(':')+1));
		map[key] = (value==0) ? TreeNode(key, PRESET_F):TreeNode(key,PRESET_T);
	}

	deque<vector<string>> ops;
	regex pattern("(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+->\\s+(\\w+)");
	while (getline(file,line)) {
		smatch match;
		vector<string> curr(4);
		regex_search(line, match, pattern);
		curr[0] = match[1];
		curr[1] = match[3];
		curr[2] = match[4];
		curr[3] = match[2];
		//each queue element will be structured [first, second, out, operation]
		ops.push_back(curr);
	}
	return {map,ops};
}

unordered_map<string,TreeNode> get_result_tree_map (pair<unordered_map<string,TreeNode>, deque<vector<string>>> input) {
	unordered_map<string,TreeNode> map = input.first;
	deque<vector<string>> q = input.second;
	while(!q.empty()) {
		vector<string> curr = q.front();
		q.pop_front();
		if (map.contains(curr[0]) && map.contains(curr[1])) {
			string op = curr[3];
			if (op == "AND") {
				map[curr[2]] = TreeNode(curr[2], Operation::AND , &map[curr[0]], &map[curr[1]]);
			}
			else if (op == "OR") {
				map[curr[2]] = TreeNode(curr[2], Operation::OR , &map[curr[0]], &map[curr[1]]);
			}
			else if (op == "XOR") {
				map[curr[2]] = TreeNode(curr[2], Operation::XOR , &map[curr[0]], &map[curr[1]]);
			}
			else {
				cout << "Unrecognized operation: " << op << '\n';
				assert(false);
			}
		}
		else {
			q.push_back(curr);
		}
	}
	return map;
}

void swap_trees (unordered_map<string,TreeNode> &map, string key1, string key2) {
	swap(map[key1], map[key2]);
	map[key1].out = key1;
	map[key2].out = key2;
}



// tactic: find a precursor tree for each item 'z<num>' which is not valid.
// another approach is to use a backtracking approach? start with z's which are incorrect, and 
// what data structure can we use for the precursor set?
void p2 (pair<unordered_map<string,TreeNode>, deque<vector<string>>> input) {
	unordered_map<string,TreeNode> map = input.first;
	long x = 0;
	long y = 0;
	long max_idx = 0;
	for (auto key: map) {
		if (key.first[0]=='x') {
			long idx = stol(key.first.substr(1));
			if (idx>max_idx) max_idx = idx;
			if (key.second.get_value()) {
				x += (1L<<idx);
				assert (x>=0);
			}
		}
		else if (key.first[0]=='y') {
			long idx = stol(key.first.substr(1));
			if (idx>max_idx) max_idx = idx;
			if (key.second.get_value()) {
				y += (1L<<idx);
				assert (y>=0);
			}
		}
	}
	long z = x+y;
	//init correct_z;
	vector<bool> correct_z(max_idx+1,false);
	for (long i=0;i<=max_idx+1; ++i) {
		correct_z[i] = get_bit_i(z, i);
	}
	unordered_map<string,TreeNode> tree_map = get_result_tree_map(input);
	//dfs to try and find the answer
	vector<pair<pair<int,vector<string>>,pair<unordered_map<string,TreeNode>,unordered_set<string>>>> stack;// <<idx_z,num_swaps>,<map,not_to_swap_set>>
	//TODO: init stack;
	while(!stack.empty()) {
		pair<pair<int,vector<string>>,pair<unordered_map<string,TreeNode>,unordered_set<string>>> curr = stack.back();
		int idx = curr.first.first; 
		vector<string> swaps = curr.first.second;
		unordered_map<string,TreeNode> curr_map = curr.second.first;
		unordered_set<string> curr_not_to_swap = curr.second.second;
		stack.pop_back();
		string z_str = "z";
		if (idx<10) z_str+="0";
		z_str+=to_string(idx);
		if (correct_z[idx]==map[z_str].get_value()) {
			unordered_set<string> next_not_to_swap = map[z_str].get_components_subset();
			copy(curr_not_to_swap.begin(),curr_not_to_swap.end(), inserter(next_not_to_swap,next_not_to_swap.end()));
			stack.push_back({{idx+1,swaps},{curr_map, next_not_to_swap}});
		}
		else {
			for (auto key: curr_map) {
				if (!curr_not_to_swap.contains(key.first)) {
					if (correct_z[idx]==key.second.get_value()) {
						unordered_map<string,TreeNode> next_map = curr_map;
						swap_trees(next_map, key.first, z_str);
						unordered_set<string> next_not_to_swap = map[z_str].get_components_subset();
						copy(curr_not_to_swap.begin(),curr_not_to_swap.end(), inserter(next_not_to_swap,next_not_to_swap.end()));
					}
				}
			}
		}
	}
}

bool verify (unordered_map<string,bool> map) {
	bool result = true;
	for (auto key: map) {
		if (key.first[0] == 'z') {
			string x_key = "x"+key.first.substr(1);
			string y_key = "y"+key.first.substr(1);
			if (map[key.first]!=(map[x_key] && map[y_key])) {
				cout << key.first << " is not correct";
				result = false;
			}
		}
	}
	return result;
}

int main () {
	pair<unordered_map<string,bool>, deque<vector<string>>> input = parse_file("d24.txt");
	long result1 = get_result(input);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}

