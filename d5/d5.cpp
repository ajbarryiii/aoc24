
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string> 
#include <vector> 
#include <unordered_map>
#include <unordered_set>

using namespace std;

pair<vector<vector<int>>, unordered_map<int, unordered_set<int>>> parse_file (string filename) {
	vector<vector<int>> update;
	// num , set of nums which must come before
	unordered_map<int,unordered_set<int>> rules;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	bool split = false;
	while (getline(file, line)) {
		if (line == "") split = true;
		else if (!split) {
			size_t pos = line.find('|');
			rules[stoi(line.substr(pos+1))].insert(stoi(line.substr(0, pos)));
		}
		else if (split) {
			stringstream ss(line);
			string num;
			vector<int> update_line;
			while(getline(ss, num, ',')) {
				update_line.push_back(stoi(num));
			}
			update.push_back(update_line);
		}
	}

	file.close();
	return pair<vector<vector<int>>, unordered_map<int, unordered_set<int>>>(update, rules);
}

int get_result( pair<vector<vector<int>>, unordered_map<int, unordered_set<int>>> input) {
	vector<vector<int>> updates = input.first;
	unordered_map<int, unordered_set<int>> rules = input.second;
	int result = 0;
	int line_no = 0;

	for(auto update : updates) {
		if (update.size() % 2 == 0) {
			cout << "update at line " << line_no << "has even length\n";
		}
		bool valid = true;
		for (int i = 0; i < update.size(); ++ i ){
			const int& curr = update[i]; 
			if (rules.find(curr) != rules.end()) {
				const unordered_set<int>& curr_set = rules[curr];
				for ( int j = i; j < update.size(); ++j) {
					if (curr_set.find(update[j]) != curr_set.end()) {
						valid = false;
						break;
					}
				}
				if (!valid) break;
			}
		}
		if (valid) {
			int mid = (update.size() / 2);
			result += update[mid];
			//cout << "line " << line_no << "is valid, adding " << update[mid] << '\n';
		}
		++line_no;
	}
	return result;
}



int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename +=".txt";
	pair<vector<vector<int>>, unordered_map<int, unordered_set<int>>> lines = parse_file(filename);
	int result = get_result(lines);
	//int result2 = get_result_2(lines);
	cout << "Result 1: " << result << '\n';
	//cout << "Result 2: " << result2 << '\n';
	return 0;
}

