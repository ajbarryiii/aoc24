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

//NOTE: maybe use two pointer approach to update, with stack.

int get_result ( pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> input) {
	int result = 0;
	vector<string> tests = input.first;
	deque<int> valid_ids;
	unordered_map<int, unordered_set<string>> input_set = input.second.first;
	int max_size = input.second.second;
	for(auto test: tests){
		int left = 0, right = 0;
		bool valid = true;
		while (valid) { 
			string curr = test.substr(left, right-left);
			if (input_set[right-left].contains(curr)) {
				valid_ids.push_back(right);
				if (right == test.size()-1) {
					cout << "string: \"" <<test<< "\" is valid \n";
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

// dfs traversal
int get_score(string s, unordered_map<int, unordered_set<string>> input_set, const int max_size) {
	int result = 0;
	vector<string> stack; // postfix stack
	stack.push_back(s);
	while(!stack.empty()) {
		string curr = stack.back();
		if (curr.size() <= max_size) {
			if (input_set[curr.size()].contains(curr)) {
				++result;
			}
		}
		stack.pop_back();
		int right = 1;
		while(right<curr.size() && right<=max_size) {
			string curr_substr = curr.substr(0,right);
			if (input_set[right].contains(curr_substr)) {
				stack.push_back(curr.substr(right));
			}
			++right;
		}
	}
	return result;
}

int get_result_p2 ( pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> input) {
	int result = 0;
	vector<string> tests = input.first;
	unordered_map<int, unordered_set<string>> input_set = input.second.first;
	int max_size = input.second.second;
	for (int i = 0; i<tests.size(); ++i) {
		int score = get_score( tests[i], input_set, max_size);
		result += score;
		cout << "The score for string : \"" << tests[i] << "\" is : " << score << '\n';
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
	int result2 = get_result_p2(input);
	cout << "Result 2: " << result2 << '\n';
	return 0;
}
