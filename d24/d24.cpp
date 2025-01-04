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
		if (value==0){
			map.insert({key,TreeNode(key, Operation::PRESET_F)});
		}
		else {
			map.insert({key,TreeNode(key, Operation::PRESET_T)});
		}
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
				map.insert({curr[2],TreeNode(curr[2], Operation::AND , &map.at(curr[0]), &map.at(curr[1]))});
			}
			else if (op == "OR") {
				map.insert({curr[2],TreeNode(curr[2], Operation::OR , &map.at(curr[0]), &map.at(curr[1]))});
			}
			else if (op == "XOR") {
				map.insert({curr[2],TreeNode(curr[2], Operation::XOR , &map.at(curr[0]), &map.at(curr[1]))});
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
	swap(map.at(key1), map.at(key2));
	map.at(key1).out = key1;
	map.at(key2).out = key2;
}

void printTree(TreeNode* root, string prefix = "", bool isLeft = true) {
	if (!root) {
		return;
	}

	cout << prefix;
	cout << (isLeft ? "├──" : "└──");

	// Print node information
	cout << "Output: " << root->out << " | Op: ";
	switch(root->operation) {
		case Operation::PRESET_F: cout << "PRESET_F"; break;
		case Operation::PRESET_T: cout << "PRESET_T"; break;
		case Operation::XOR: cout << "XOR"; break;
		case Operation::AND: cout << "AND"; break;
		case Operation::OR: cout << "OR"; break;
		default: cout << "UNKNOWN";
	}
	cout << " | Value: " << (root->get_value() ? "TRUE" : "FALSE") << endl;

	// Print children
	printTree(root->left_input, prefix + (isLeft ? "│   " : "    "), true);
	printTree(root->right_input, prefix + (isLeft ? "│   " : "    "), false);
}

void debug(pair<unordered_map<string,TreeNode>, deque<vector<string>>> input) {
	unordered_map<string,TreeNode> tree_map = get_result_tree_map(input);
	while (true) {
		string i;
		cout << "Enter the item you want to debug\n";
		cin >> i;
		if (tree_map.contains(i)) printTree(&tree_map.at(i));
		else {
			break;
		}
	}
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
	string z_max_idx = "z"+to_string(max_idx+1);
	if (!map.contains(z_max_idx)) {
		map.insert({z_max_idx,TreeNode(z_max_idx, Operation::PRESET_F)});
	}
	unordered_map<string,TreeNode> tree_map = get_result_tree_map(input);
	while (true) {
		string to_check;
		cout << "\ninput the node you want to check:\n";
		cin >> to_check;
		if (tree_map.contains(to_check)) {
			if (to_check[0] == 'z') {
				int integer_part = stoi(to_check.substr(1));
				cout << "Correct value: " <<correct_z[integer_part] <<'\n';
			}
			printTree(&tree_map.at(to_check));
		}
		else {
			break;
		}
	}
}

int main () {
	pair<unordered_map<string,bool>, deque<vector<string>>> input = parse_file("d24.txt");
	long result1 = get_result(input);
	cout << "Result 1: " << result1 << '\n';
	pair<unordered_map<string,TreeNode>, deque<vector<string>>> input_2 = parse_file_p2("d24.txt");
	cout << "file parsed successfully for p2\n";
	p2(input_2);
	return 0;
}

