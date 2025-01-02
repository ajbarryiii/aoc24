#include <vector> 
#include <array>
#include <string> 
#include <fstream>
#include <string> 
#include <cassert>
#include <iostream>

using namespace std;

pair<vector<array<int,5>>,vector<array<int,5>>> parse_file (string filename) {
	vector<array<int,5>> keys, locks;
	ifstream file(filename);
	string line;
	array<int,5> curr_map;
	assert(file.is_open());
	int map_line_no = 0;
	bool key = true;
	char to_count = '#';
	while (getline(file,line)) {
		if (line.empty()) {
			map_line_no = 0;
			if (key) {
				keys.emplace_back(curr_map);
			}
			else {
				locks.emplace_back(curr_map);
			}
			//reset the array
			curr_map.fill(0);
			key = true;
			to_count = '#';
		}
		else {
			if (map_line_no == 0) {
				if (line[0]=='#') {
					key=false;
					to_count = '.';
				}
			}
			for (int i=0; i<line.size();++i) {
				if (line[i]==to_count) {
					curr_map[i]++;
				}
			}
			++map_line_no;
		}
	}
	if (key) {
		keys.emplace_back(curr_map);
	}
	else {
		locks.emplace_back(curr_map);
	}
	return {keys,locks};
}

int get_result (const pair<vector<array<int,5>>,vector<array<int,5>>> input) {
	int result = 0;
	const vector<array<int,5>> keys = input.first, locks = input.second;
	for (int i=0; i<locks.size(); ++i) {
		for (int j=0; j<keys.size(); ++j) {
			bool valid = true;
			for (int idx=0; idx<5;++idx) {
				if (keys[j][idx]>locks[i][idx]) {
					valid = false;
					break;
				}
			}
			if (valid) result++;
		}
	}
	return result;
}

int main () {
	pair<vector<array<int,5>>,vector<array<int,5>>> keys_locks = parse_file("d25.txt");
	vector<array<int,5>> keys = keys_locks.first, locks = keys_locks.second;
	int result1 = get_result(keys_locks);
	cout << "Result 1: " << result1<<'\n';
	return 0;
}
