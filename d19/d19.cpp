#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> parse_file (string filename) {
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
	return {tests,{subsequences, max_len_subsequence}};
}

//NOTE: use memorization to store sequences that we have found are possible?
//what if we get all unique lengths of the strings, and build them up.
//THIS WAS A BAD IDEA,

void compute_subs (unordered_map<int, unordered_set<string>>& input_set ,int max_size){
	assert(max_size>1);
	for (int i = 1; i<=max_size; ++i) {
		for (int j = 1; j<i; ++j) {
			for (auto setj: input_set[j]) {
				for (auto seti_j: input_set[i-j]) {
					input_set[i].insert(setj+seti_j);

				}
			}
		}
	}
}

//NOTE: maybe use two pointer approach to update, with stack.

int get_result ( pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> input) {
	int result = 0;
	vector<string> tests = input.first;
	deque<int> valid_ids;
	unordered_map<int, unordered_set<string>> input_set = input.second.first;
	int max_size = input.second.second;
	//compute_subs(input_set, max_size); //NOTE: re-add this maybe?
	for(auto test: tests){
		int left = 0, right = 0;
		bool valid = true;
		while (valid) { 
			string curr = test.substr(left, right-left);
			if (input_set[right-left].contains(curr)) {
				valid_ids.push_back(right);
				if (right == test.size()-1) {
					result++;
					break;
				}
			}
			if (right<test.size()-1 && right-left <= max_size) {
				++right;
			}
			else {
				if (valid_ids.empty()) {
					valid = false;
				}
				else {
					left = valid_ids.back();
					right = left;
					valid_ids.pop_back();
				}
			}
		}
		valid_ids.clear();
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename:\n";
	cin >> filename;
	filename += ".txt";
	pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> input = parse_file(filename);
	int result1 = get_result(input);
	cout << "Result 1: " << result1 << '\n';
	return 0;
}
