#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <unordered_set>

using namespace std;

pair<vector<string>,unordered_map<int, unordered_set<string>>> parse_file (string filename) {
	unordered_map<int,unordered_set<string>> subsequences;
	unordered_set<string> dupilicates;
	vector<string> tests;
	int max_len = 0;
	int max_len_subsequence = 0;
	string line;
	ifstream file(filename);
	assert(file.is_open());

	while(getline(file, line)) {
		if (!line.empty()) {
			stringstream ss(line);
			string stripes;
			while (getline(ss, stripes, ',')) {
				stripes.erase(0,stripes.find_first_not_of(' '));
				subsequences[stripes.size()].insert(stripes);
				if (stripes.size() > max_len_subsequence) {
					max_len_subsequence = stripes.size();
				}
			}
		}
		else {
			break;
		}
	}
	while(getline(file, line)) {
		tests.push_back(line);
		if (line.size()>max_len){
			max_len = line.size();
		}
		if (dupilicates.contains(line)) {
			cout << "String: " << line << " appears more than once. \n";
		}
		else {
			dupilicates.insert(line);
		}
	}
	cout << "max subsequence len: " << max_len_subsequence << '\n';
	cout << "max sequence len: " << max_len << '\n';
	return {tests,subsequences};
}

//NOTE: use memorization to store sequences that we have found are possible?
//what if we get all unique lengths of the strings, and build them up.

void compute_subs (unordered_map<int, unordered_set<string>>& input_set ,int max_size){
	assert(max_size>1);
	for (int i = 1; i<max_size; ++i) {
		cout << "Working on subsequences of size: " << i << '\n';
		for (int j = 1; j<i; ++j) {
			for (auto setj: input_set[j]) {
				for (auto seti_j: input_set[i-j]) {
					input_set[i].insert(setj+seti_j);
				}
			}
		}
	}
	cout << "complete\n";
}

int get_result ( pair<vector<string>,unordered_map<int, unordered_set<string>>> input, int max_size) {
	int result = 0;
	vector<string> tests = input.first;
	unordered_map<int, unordered_set<string>> input_set = input.second;
	compute_subs(input_set, max_size);
	for(auto test: tests){
		if (input_set[test.size()].contains(test)) {
			++result;
		}
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename:\n";
	cin >> filename;
	filename += ".txt";
	pair<vector<string>,unordered_map<int, unordered_set<string>>> input = parse_file(filename);
	int result1 = get_result(input, 30);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}
