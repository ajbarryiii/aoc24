#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <algorithm>

using namespace std;

pair<vector<int>, vector<int>> parse_file (string filename) {
	pair<vector<int>, vector<int>> result;
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
			result.second.push_back(val);
		}
		++which_vec;
	}
	file.close();
	return result;
}

int compare_lists(pair<vector<int>,vector<int>> input) {
	vector<int> list1 = input.first, list2 = input.second;
	int result = 0;

	if (list1.size() != list2.size()) {
		cerr << "err: lists do not have same size";
	}

	sort(list1.begin(), list1.end());
	sort(list2.begin(), list2.end());

	for(int i = 0; i < list1.size(); ++i) {
		result += abs(list1[i] - list2[i]);
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	pair<vector<int>,vector<int>> parsed_files = parse_file(filename);
	int result = compare_lists(parsed_files);
	cout << "Result: " << result << '\n';

	return 0;
}

