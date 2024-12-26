#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
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

// what if we use a bfs instead?
int get_score ( string s, unordered_map<int, unordered_set<string>> input_set, const int max_size){ //, unordered_map<string,int>& visited) {
	deque<pair<string,string>> q; //(prefix, postfix stack);
	unordered_map<string,int> visited;
	q.push_back({s,""});
	while (!q.empty()) {
		string post = q.front().second;
		string prev = q.front().first;
		q.pop_front();
		int left = 1;
		string acc;
		while (!prev.empty() && left<=max_size) {
			acc = prev.back()+acc;
			prev.pop_back();
			if (input_set[left].contains(acc)){
				q.push_back({prev, acc+post});
				visited[acc+post]++;
			}
			++left;
		}
	}
	return visited[s];
}

long get_score_dp ( string s, unordered_map<int, unordered_set<string>> input_set, const int max_size){ //, unordered_map<string,int>& visited) {
	vector<long> dp(s.size()+1,0);
	string acc;
	int left = 0, right = 1;
	dp[0]=1;
	for (int i=1; i<=dp.size(); ++i) {
		for (int j=1; j<=min(i,max_size);++j) {
			if (i-j>=0) {
				string sub = s.substr(i-j,j);
				if (input_set[j].contains(sub)) {
					dp[i]+=dp[i-j];
				}
			}
		}
	}
	return dp.back();
}

long get_result_p2 (pair<vector<string>,pair<unordered_map<int, unordered_set<string>>,int>> input) {
	long result = 0; 
	vector<string> tests = input.first;
	unordered_map<int, unordered_set<string>> input_set = input.second.first;
	int max_size = input.second.second;
	for (auto str:tests) {
		long score = get_score_dp(str,input_set,max_size);
		assert(score>=0);
		result += score;
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
	long result2 = get_result_p2(input);
	cout << "Result 1: " << result1 << '\n';
	cout << "TEST P2: " << result2 << "\n";
	return 0;
}
