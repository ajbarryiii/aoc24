#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <algorithm>
#include <sstream>

using namespace std;

vector<vector<int>> parse_file (string filename) {
	vector<vector<int>> result;
	ifstream file(filename);

	if (!file.is_open()) {
	 cerr << "Err: could not open file \n";
	}

	string line;
	while (getline(file, line)) {
		vector<int> line_vec;
		istringstream stream(line);
		string word;
		while (stream >> word) {
			line_vec.push_back(stoi(word));
		}
		result.push_back(line_vec);
	}
	file.close();
	return result;
}

int isSafe (vector<int> line) {
	if (line.size() < 2){
		cout << "line has len < 2"<< '\n';
		return 1;
	}
	
	// case of increasing
	if (line[0] < line[1]) {
		for(int i = 0; i < line.size()-1; ++i) {
			if (!(line[i] < line[i+1] and line[i] >= line[i+1] - 3)) {
				return 0;
			}
		}
		return 1;
	}
	// case of decreasing 
	else if (line[0] > line[1]) {
		for(int i = 0; i < line.size()-1; ++i) {
			if (!(line[i] > line[i+1] and line[i] <= line[i+1] +3 )) {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}

std::vector<int> get_all_except(const std::vector<int>& input, size_t index) {
	std::vector<int> result;
	for (size_t i = 0; i < input.size(); ++i) {
		if (i != index) {
			result.push_back(input[i]);
		}
	}
	return result;
}

int isSafeDampener (vector<int> line) {
	if (isSafe(line)){
		return 1;
	}
	else {
		for ( int i = 0; i < line.size(); ++i) {
			vector <int> temp = get_all_except(line, i);
			if (isSafe(temp) == 1){
				return 1;
			}
		}
		return 0;
	}
}


int compute_ans ( vector<vector<int>> input) {
	int result = 0;
	for(auto i : input) {
		result += isSafe(i);
	}
	return result;
}

int compute_ans_damp ( vector<vector<int>> input) {
	int result = 0;
	for(auto i : input) {
		result += isSafeDampener(i);
	}
	return result;
}

int main() {
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	vector<vector<int>> lines = parse_file(filename);
	int result = compute_ans(lines);
	int result_2 = compute_ans_damp(lines);
	cout << "Result 1: " << result << '\n';
	cout << "Result 2: " << result_2 << '\n';
	return 0;
}


