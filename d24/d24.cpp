#include <cassert>
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <regex>
#include <deque>
#include <bitset>
#include <unordered_map>

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

long get_result (pair<unordered_map<string,bool>, deque<vector<string>>> input) {
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
	long result = 0;
	for (auto key: map) {
		if (key.first[0]=='z') {
			long idx = stol(key.first.substr(1));
			if (key.second) {
				long init = 1;
				result += (init<<idx);
			}
		}
		assert (result>=0);
	}
	return result;
}

int main () {
	pair<unordered_map<string,bool>, deque<vector<string>>> input = parse_file("d24.txt");
	long result1 = get_result(input);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}

