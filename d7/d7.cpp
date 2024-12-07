#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string> 
#include <vector> 
#include <deque>

using namespace std;

vector<pair<int, vector<int>>> parse_file (string filename) {
	vector<pair<int, vector<int>>> map;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	while (getline(file, line)) {
		size_t split = line.find(':');
		vector<int> line_nums;
		string post = line.substr(split+1);
		istringstream iss(post);
		int value;
		while (iss >> value) {
			line_nums.push_back(value);
			if ( value < 0) {
				cout << "value < 0 found \n";
			}
		}
		map.push_back(pair<int,vector<int>>(stoi(line.substr(0, split)), line_nums)); 
	}

	file.close();
	return map;
}

bool dfs( const int &target , const vector<int> &nums) {
	deque<int> q;
	q.push_back(nums[0]);
	cout << " target : " << target << '\n';
	for ( int i = 1; i<nums.size(); ++i) {
		if (!q.empty()){
			const int q_len = q.size();
			for (int j = 0; j < q.size(); ++j) {
				int curr = q.front();
				q.pop_front();
				if ( curr * nums[i] <= target) {
					q.push_back(curr*nums[i]);
					cout << curr * nums[i]<< '\n';
				}
				if (curr + nums[i] <= target) {
					q.push_back(curr+nums[i]);
					cout << curr * nums[i]<< '\n';
				}
			}
		}
		else {
			return false;
		}
	}
	return false;
}


int get_result( const vector<pair<int,vector<int>>> map ) {
	int result = 0;
	for( auto line : map ) {
		bool found = dfs(line.first , line.second);
		if (found) ++result;
	}
	return result;
}


int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	filename +=".txt";
	vector<pair<int, vector<int>>> lines = parse_file(filename);
	//int result = get_result(lines);
	bool test = dfs(lines.back().first, lines.back().second);
	//cout << "Result 1: " << result << '\n';
	//cout << "Result 2: " << result2 << '\n';
	return 0;
}

