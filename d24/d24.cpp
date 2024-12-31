#include <cassert>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <regex>
#include <deque>
#include <unordered_map>
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

// tactic: find a precursor tree for each item 'z<num>' which is not valid.
// another approach is to use a backtracking approach? start with z's which are incorrect, and 
// what data structure can we use for the precursor set?
bool p2 (pair<unordered_map<string,bool>, deque<vector<string>>> input) {
	deque<vector<string>> q = input.second;
	unordered_map<string,bool> map = get_result_map(input);
	long x = 0;
	long y = 0;
	for (auto key: map) {
		if (key.first[0]=='x') {
			long idx = stol(key.first.substr(1));
			if (key.second) {
				x += (1L<<idx);
				assert (x>=0);
			}
		}
		else if (key.first[0]=='y') {
			long idx = stol(key.first.substr(1));
			if (key.second) {
				y += (1L<<idx);
				assert (y>=0);
			}
		}
	}
	long z = x+y;
	//init correct_z;
	unordered_map<string,bool> correct_z;
	for (long i=0;i<sizeof(z)*8; ++i) {
		string key_z = "z";
		string numeric_part = to_string(i);
		if (numeric_part.size()==1) {
			key_z += "0"+numeric_part;
		}
		else {
			key_z += numeric_part;
		}
		correct_z[key_z] = get_bit_i(z, i);
	}
	return false;
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

