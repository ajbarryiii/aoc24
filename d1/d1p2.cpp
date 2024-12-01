#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <algorithm>
#include <unordered_map>

using namespace std;

pair<vector<int>, unordered_map<int, int>> parse_file (string filename) {
	pair<vector<int>, unordered_map<int, int>> result;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string num; 
	int which_vec = 0;
	while (file >> num) {
		int val = stoi(num);
		if (which_vec %2 == 0) {
			result.first.push_back(val);
		}
		else {
			++result.second[val];
		}
		++which_vec;
	}
	file.close();
	return result;
}

int compare_lists(pair<vector<int>,unordered_map<int,int>> input) {
	vector<int> list = input.first;
	unordered_map<int,int> map = input.second;
	int result = 0;

	for ( int i = 0; i < list.size(); ++i) {
		result += list[i] * map[list[i]];
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	pair<vector<int>,unordered_map<int,int>> parsed_files = parse_file(filename);
	int result = compare_lists(parsed_files);
	cout << "Result: " << result << '\n';

	return 0;
}


