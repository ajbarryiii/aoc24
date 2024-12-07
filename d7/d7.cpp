#include <iostream> 
#include <fstream> 
#include <sstream>
#include <stdexcept>
#include <string> 
#include <vector> 
#include <deque>
#include <cassert>

using namespace std;

vector<pair<long, vector<long>>> parse_file (string filename) {
	vector<pair<long, vector<long>>> map;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	while (getline(file, line)) {
		size_t split = line.find(':');
		vector<long> line_nums;
		string post = line.substr(split+1);
		istringstream iss(post);
		long value;
		while (iss >> value) {
			line_nums.push_back(value);
			if ( value < 0) {
				cout << "value < 0 found \n";
			}
		}
		long tgt;
		try {
			tgt = stol(line.substr(0, split));
		}
		catch (const out_of_range& e) {
			cerr << "error: numer out of range in line: " << line << '\n';
			continue;
		}
		map.push_back(pair<long,vector<long>>(tgt, line_nums)); 
	}

	file.close();
	return map;
}

bool dfs( const long &target , const vector<long> &nums) {
	deque<long> q;
	q.push_back(nums[0]);
	for ( int i = 1; i<nums.size(); ++i) {
		if (!q.empty()){
			const int q_len = q.size();
			for (int j = 0; j < q_len; ++j) {
				long curr = q.front();
				q.pop_front();
				if ( (curr * nums[i]) <= target) {
					q.push_back(curr*nums[i]);
				}
				if ((curr + nums[i]) <= target) {
					q.push_back(curr+nums[i]);
				}
			}
		}
		else {
			return false;
		}
	}
	for (auto i : q) {
		if (i == target) return true;
	}
	return false;
}

bool dfs2 (const long &target, const vector<long> &nums) {
	deque<long> q;
	q.push_back(nums[0]);
	for ( int i = 1; i<nums.size(); ++i) {
		if (!q.empty()){
			const int q_len = q.size();
			string nums_i_str = to_string(nums[i]);
			for (int j = 0; j < q_len; ++j) {
				long curr = q.front();
				assert (curr > 0);
				q.pop_front();
				if ( (curr * nums[i]) <= target) {
					q.push_back(curr*nums[i]);
				}
				if ((curr + nums[i]) <= target) {
					q.push_back(curr+nums[i]);
				}
				if (stol(to_string(curr) + nums_i_str) <= target) {
					q.push_back(stol(to_string(curr) + nums_i_str));
				}
			}
		}
		else {
			return false;
		}
	}
	for (auto i : q) {
		assert (i > 0);
		if (i == target) return true;
	}
	return false;

}

long get_result( const vector<pair<long,vector<long>>> map ) {
	long result = 0;
	for( auto line : map ) {
		bool found = dfs(line.first , line.second);
		if (found){
			result += line.first;
		}
	}
	return result;
}

long get_result_p2( const vector<pair<long,vector<long>>> map ) {
	long result = 0;
	for( auto line : map ) {
		bool found = dfs2(line.first , line.second);
		if (found){
			result += line.first;
			assert ( result > 0);
		}
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename +=".txt";
	vector<pair<long, vector<long>>> lines = parse_file(filename);
	long result = get_result(lines);
	long result2 = get_result_p2(lines);
	cout << "Result 1: " << result << '\n';
	cout << "Result 2: " << result2 << '\n';
	return 0;
}

